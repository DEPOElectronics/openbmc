#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <reimu.h>

const char *config_dir = "/etc/default/obmc/hwmon/devices/platform/ahb/ahb--apb/ahb--apb--bus";
const char *supported_devices[] = { "lm96163", "pmbus", "emc2305", "l_pcs_i2c", NULL };

static int instantiate(const char *compatible, int bus, int reg)
{
    if (!reimu_is_in_dict(supported_devices, compatible))
    {
        printf(" - device is unsupported, skipping\n");
        return 1;
    }

    char sysfspath[512];
    char sysfsdata[256];
    snprintf(sysfspath, 511, "/sys/class/i2c-adapter/i2c-%d/new_device", bus);
    snprintf(sysfsdata, 255, "%s 0x%02x\n", compatible, reg);
    if(reimu_writefile(sysfspath, sysfsdata, strlen(sysfsdata)))
    {
        printf("- instantiating failed (%s), skipping\n", strerror(errno));
        return 2;
    }

    snprintf(sysfspath, 511, "/sys/class/i2c-adapter/i2c-%d/%d-%04x/hwmon", bus, bus, reg);
    if (reimu_chkdir(sysfspath))
    {
        printf("- no hwmon directory exported (%s), skipping\n", strerror(errno));
        return 3;
    }

    printf(" - instantiated device %s at %d-%04x\n", compatible, bus, reg);
    return 0;
}

static int deinstantiate(const char *compatible, int bus, int reg)
{
    if (!reimu_is_in_dict(supported_devices, compatible))
    {
        printf(" - device is unsupported, skipping\n");
        return 1;
    }

    char sysfspath[512];
    char sysfsdata[256];
    snprintf(sysfspath, 511, "/sys/class/i2c-adapter/i2c-%d/delete_device", bus);
    snprintf(sysfsdata, 255, "0x%02x\n", reg);
    if(reimu_writefile(sysfspath, sysfsdata, strlen(sysfsdata)))
    {
        printf("- deinstantiating failed (%s), skipping\n", strerror(errno));
        return 2;
    }

    printf(" - deinstantiated device %s at %d-%04x\n", compatible, bus, reg);
    return 0;
}

static void detect_addresses(int i2c, int *apb, int *bus)
{
    char bus_path[1024], *real_path;
    snprintf(bus_path, 1023, "/sys/class/i2c-adapter/i2c-%d/device", i2c);
    if((real_path = realpath(bus_path, NULL)) == NULL) reimu_cancel(40, "Can't get real path of device node %s: %s\n", bus_path, strerror(errno));

    char *apb_path = strchr(real_path, '@');
    if (apb_path != NULL)
    {
        ++apb_path;
        char *i2c_path = strchr(apb_path, '/');
        if (i2c_path != NULL)
        {
            *i2c_path = '\0';
            ++i2c_path;
            char *i2c_path_end = strchr(i2c_path, '.');
            if (i2c_path_end != NULL)
            {
                *i2c_path_end = '\0';

                char *apb_end, *bus_end;
                *apb = strtoul(apb_path, &apb_end, 16);
                *bus = strtoul(i2c_path, &bus_end, 16);
                if (!*apb_end && !*bus_end)
                {
                    free(real_path);
                    return;
                }
            }
        }
    }
    reimu_cancel(41, "Can't parse real path of device node %s\n", bus_path);
    free(real_path);
}

static void create_sensor(int node, const char *nodename, traverse_callback_t unused1, const void *data, int unused2)
{
    if (nodename == NULL) reimu_cancel(22, "Error reading name of node 0x%08x\n", node);

    const char *devlabel = (const char *)data;
    const char *type = NULL;
    if (!strncmp(nodename, "fan", 3))   type = "fan";
    if (!strncmp(nodename, "temp", 4))  type = "temp";
    if (!strncmp(nodename, "in", 2))    type = "in";
    if (!strncmp(nodename, "curr", 4))  type = "curr";
    if (!strncmp(nodename, "power", 5)) type = "power";

    if (type != NULL)
    {
        const int *preg = reimu_getprop(node, "reg", 1, 23, "Error reading reg value from node 0x%08x:", node);
        int reg = *preg >> 24;
        const char *label = reimu_getprop(node, "label", 0, 24, "Error reading label value from node 0x%08x:", node);
        const char *status = reimu_getprop(node, "status", 1, 29, "Error reading status value from node 0x%08x:", node);

        if (!strcmp(status, "okay"))
        {
            char sensor_label[1024];
            snprintf(sensor_label, 1023, "%s %s", devlabel, label);

            for (char *p = sensor_label; *p; ++p)
            {
                /* The label shall comply with "Valid Object Paths" of D-Bus Spec, that shall only contain the ASCII characters "[A-Z][a-z][0-9]_". */
                if ((*p >= 'A') && (*p <= 'Z')) continue;
                if ((*p >= 'a') && (*p <= 'z')) continue;
                if ((*p >= '0') && (*p <= '9')) continue;
                *p = '_';
            }
            printf(" - adding sensor %s as %s%d (%s)\n", nodename, type, reg, sensor_label);
            reimu_write_text_file("LABEL_%s%d=%s\n", type, reg, sensor_label);

            const char *warnlo = reimu_getprop(node, "min_alert", 1, 25, "Error reading min alert value from node 0x%08x:", node);
            const char *warnhi = reimu_getprop(node, "max_alert", 1, 26, "Error reading max alert value from node 0x%08x:", node);
            const char *critlo = reimu_getprop(node, "min_crit", 0, 27, "Error reading min crit value from node 0x%08x:", node);
            const char *crithi = reimu_getprop(node, "max_crit", 0, 28, "Error reading max crit value from node 0x%08x:", node);

            printf(" - %s: warn %s..%s", nodename, warnlo, warnhi);
            reimu_write_text_file("WARNLO_%s%d=%s\nWARNHI_%s%d=%s\n", type, reg, warnlo, type, reg, warnhi);
            char *event = "WARNHI,WARNLO";

            if (reimu_is_prop_empty(crithi) || reimu_is_prop_empty(critlo))
            {
                printf(", crit %s..%s", critlo, crithi);
                reimu_write_text_file("CRITLO_%s%d=%s\nCRITHI_%s%d=%s\n", type, reg, critlo, type, reg, crithi);
                event = "WARNHI,WARNLO,CRITHI,CRITLO";
            }
            printf("\n");
            reimu_write_text_file("EVENT_%s%d=\"%s\"\nASYNC_READ_TIMEOUT_%s%d=\"1000\"\n\n", type, reg, event, type, reg);
        }
        else
        {
            printf(" - %s is disabled, skipping\n", nodename);
        }
    }
    else
    {
        printf(" - %s is not of supported types, skipping\n", nodename);
    }
}

static void create_config(int parent, int bus, int dev, const char *devlabel)
{
    int apb_addr, bus_addr;
    detect_addresses(bus, &apb_addr, &bus_addr);

    char config_path[1024];
    snprintf(config_path, 1023, "%s@%08x/%08x.i2c-bus/i2c-%d", config_dir, apb_addr, bus_addr, bus);
    if(reimu_recurse_mkdir(config_path)) reimu_cancel(31, "Error while creating directory for config file for device %d-%04x (%s): %s\n", bus, dev, config_path, strerror(errno));
    snprintf(config_path, 1023, "%s@%08x/%08x.i2c-bus/i2c-%d/%d-%04x.conf", config_dir, apb_addr, bus_addr, bus, bus, dev);
    if (reimu_create_text_file(config_path)) reimu_cancel(32, "Error while creating config file for device %d-%04x (%s): %s\n", bus, dev, config_path, strerror(errno));

    if(reimu_for_each_subnode(parent, NULL, (const void *)devlabel, 0, create_sensor))
    {
        reimu_cancel(21, "Error traversing i2c device %d-%04x (%s), node 0x%08x\n", bus, dev, devlabel, parent);
    }
    reimu_close_text_file();
}

enum serv_op { SERVICE_START, SERVICE_STOP, SERVICE_OP_UNKNOWN };

static void instantiator_callback(const char *pcompatible, int node, int bus, int reg, const char *label, const void *data)
{
    enum serv_op op = *(const enum serv_op*)data;
    switch(op)
    {
        case SERVICE_START:
            create_config(node, bus, reg, label);
            instantiate(pcompatible, bus, reg);
            break;

        case SERVICE_STOP:
            deinstantiate(pcompatible, bus, reg);
            break;
    }
}

int main(int argc, char *argv[])
{
    enum serv_op op = SERVICE_OP_UNKNOWN;
    if (argc > 1)
    {
        if (!strcmp(argv[1], "start")) op = SERVICE_START;
        if (!strcmp(argv[1], "stop")) op = SERVICE_STOP;
    }
    if (op == SERVICE_OP_UNKNOWN) reimu_cancel(3, "Incorrect parameters, use {start|stop}\n");

    reimu_traverse_all_i2c((void *)&op, instantiator_callback);
    return 0;
}
