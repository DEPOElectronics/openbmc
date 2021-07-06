#include <stdio.h>
#include <stdarg.h>
#include <libfdt.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

const char *config_dir = "/etc/default/obmc/hwmon/devices/platform/ahb/ahb--apb/ahb--apb--bus";

const char *devtree_path = "/var/volatile/motherboard_devtree.dtb";

char *supported_devices[] = { "lm96163", "pmbus", "emc2305", "l_pcs_i2c", NULL };

void cancel(int num, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(num);
}

static int readfile(const char *name, char **p_buf, long *p_size)
{
    if(p_buf == NULL) return 8;

    FILE *f = fopen(name, "r");
    if (f == NULL) return 2;

    long size = 4096;
    if(p_size != NULL)
    {
        if (fseek(f, 0L, SEEK_END)) { fclose(f); return 3; }
        if ((size = ftell(f)) <= 0) { fclose(f); return 4; }
        rewind(f);
    }

    if ((*p_buf = malloc(size)) == NULL) { fclose(f); return 5; }

    if (p_size != NULL) *p_size = size;
    size = fread(*p_buf, size, 1, f);
    if((p_size != NULL) && (size != 1)) { free(*p_buf); fclose(f); return 6; }
    if(fclose(f)) { free(*p_buf); return 7; }

    return 0;
}

static const char *check_args(int argc, char *argv[], const char *def)
{
    return (argc > 1) ? argv[1] : def;
}

static int is_supported(const char *name)
{
    for (int i = 0; supported_devices[i] != NULL; ++i)
    {
        if (!strcmp(supported_devices[i], name)) return 1;
    }
    return 0;
}

static int writefile(const char *name, const void *buf, long size)
{
    if(buf == NULL) return 8;

    FILE *f = fopen(name, "w");
    if (f == NULL) return 2;

    if(fwrite(buf, size, 1, f) != 1) { fclose(f); return 6; }
    if(fclose(f)) return 7;

    return 0;
}

static int chkdir(const char *path)
{
    DIR *d;
    if((d = opendir(path)) == NULL) return 1;
    if(closedir(d) != 0) return 2;
    return 0;
}

static int instantiate(const char *compatible, int bus, int reg)
{
    if (!is_supported(compatible))
    {
        printf(" - device is unsupported, skipping\n");
        return 1;
    }

    char sysfspath[512];
    char sysfsdata[256];
    snprintf(sysfspath, 511, "/sys/class/i2c-adapter/i2c-%d/new_device", bus);
    snprintf(sysfsdata, 255, "%s 0x%02x\n", compatible, reg);
    if(writefile(sysfspath, sysfsdata, strlen(sysfsdata)))
    {
        printf("- instantiating failed (%s), skipping\n", strerror(errno));
        return 2;
    }

    snprintf(sysfspath, 511, "/sys/class/i2c-adapter/i2c-%d/%d-%04x/hwmon", bus, bus, reg);
    if (chkdir(sysfspath))
    {
        printf("- no hwmon directory exported (%s), skipping\n", strerror(errno));
        return 3;
    }

    printf(" - instantiated device %s at %d-%04x\n", compatible, bus, reg);
    return 0;
}

char *dtb;

static void free_dtb(void)
{
    free(dtb);
}

const char *empty = "";

static const void *getprop(int node, const char *name, int mandatory, int failval, const char *fmt, ...)
{
    int err;
    const void *ptr = fdt_getprop(dtb, node, name, &err);
    if (ptr == NULL)
    {
        if ((err == -FDT_ERR_NOTFOUND) && !mandatory)
        {
            ptr = empty;
        }
        else
        {
            va_list ap;
            va_start(ap, fmt);
            vfprintf(stderr, fmt, ap);
            va_end(ap);
            fprintf(stderr, " %s\n", fdt_strerror(err));
            exit(failval);
        }
    }
    return ptr;
}

FILE *f_config = NULL;

static void close_config_file(void)
{
    if (f_config) fclose(f_config);
    f_config = NULL;
}

static void detect_addresses(int i2c, int *apb, int *bus)
{
    char bus_path[1024], *real_path;
    snprintf(bus_path, 1023, "/sys/class/i2c-adapter/i2c-%d/device", i2c);
    if((real_path = realpath(bus_path, NULL)) == NULL) cancel(40, "Can't get real path of device node %s: %s\n", bus_path, strerror(errno));

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
    cancel(41, "Can't parse real path of device node %s\n", bus_path);
    free(real_path);
}

static int recurse_mkdir(char *path)
{
    if (!chkdir(path)) return 0;
    for (char *sym = path + 1; *sym; ++sym)
    {
        if (*sym == '/')
        {
            *sym = '\0';
            if (chkdir(path))
            {
                if(mkdir(path, 0777)) return -1;
            }
            *sym = '/';
        }
    }
    if(mkdir(path, 0777)) return -1;
    return 0;
}

static void create_config_file(int bus, int dev)
{
    if (f_config) cancel(30, "Error while creating config file for device %d-%04x: Handle already locked\n", bus, dev);

    int apb_addr, bus_addr;
    detect_addresses(bus, &apb_addr, &bus_addr);

    char config_path[1024];
    snprintf(config_path, 1023, "%s@%08x/%08x.i2c-bus/i2c-%d", config_dir, apb_addr, bus_addr, bus);
    if(recurse_mkdir(config_path)) cancel(31, "Error while creating directory for config file for device %d-%04x (%s): %s\n", bus, dev, config_path, strerror(errno));

    snprintf(config_path, 1023, "%s/i2c-%d/%d-%04x.conf", config_dir, bus, bus, dev);
    f_config = fopen(config_path, "w");
    if (f_config == NULL) cancel(32, "Error while creating config file for device %d-%04x: %s\n", bus, dev, strerror(errno));
}

static void write_config_file(const char *fmt, ...)
{
    if (!f_config) cancel(40, "Error while writing config file: File isn't opened\n");
    va_list ap;
    va_start(ap, fmt);
    int rv = vfprintf(f_config, fmt, ap);
    va_end(ap);
    if (rv < 0) cancel(41, "Error while writing config file: vfprintf() returned %d\n", rv);
}

static void create_config(int parent, int bus, int dev, const char *devlabel)
{
    create_config_file(bus, dev);

    int node;
    fdt_for_each_subnode(node, dtb, parent)
    {
        int err;

        const char *nodename = fdt_get_name(dtb, node, &err);
        if (nodename == NULL) cancel(22, "Error reading name of node 0x%08x: %s\n", node, fdt_strerror(err));

        const char *type = NULL;
        if (!strncmp(nodename, "fan", 3))   type = "fan";
        if (!strncmp(nodename, "temp", 4))  type = "temp";
        if (!strncmp(nodename, "in", 2))    type = "in";
        if (!strncmp(nodename, "curr", 4))  type = "curr";
        if (!strncmp(nodename, "power", 5)) type = "power";

        if (type != NULL)
        {
            const int *preg = getprop(node, "reg", 1, 23, "Error reading reg value from node 0x%08x:", node);
            int reg = *preg >> 24;
            const char *label = getprop(node, "label", 0, 24, "Error reading label value from node 0x%08x:", node);
            const char *status = getprop(node, "status", 1, 29, "Error reading status value from node 0x%08x:", node);

            if (!strcmp(status, "okay"))
            {
                char sensor_label[1024];
                snprintf(sensor_label, 1023, "%s %s", devlabel, label);
                for (char *p = sensor_label; *p; ++p) if (*p == ' ') *p = '_';
                printf(" - adding sensor %s as %s%d (%s)\n", nodename, type, reg, sensor_label);
                write_config_file("LABEL_%s%d=%s\n", type, reg, sensor_label);

                const char *warnlo = getprop(node, "min_alert", 1, 25, "Error reading min alert value from node 0x%08x:", node);
                const char *warnhi = getprop(node, "max_alert", 1, 26, "Error reading max alert value from node 0x%08x:", node);
                const char *critlo = getprop(node, "min_crit", 0, 27, "Error reading min crit value from node 0x%08x:", node);
                const char *crithi = getprop(node, "max_crit", 0, 28, "Error reading max crit value from node 0x%08x:", node);

                printf(" - %s: warn %s..%s", nodename, warnlo, warnhi);
                write_config_file("WARNLO_%s%d=%s\nWARNHI_%s%d=%s\n", type, reg, warnlo, type, reg, warnhi);
                if ((crithi != empty) || (critlo != empty))
                {
                    printf(", crit %s..%s", critlo, crithi);
                    write_config_file("CRITLO_%s%d=%s\nCRITHI_%s%d=%s\n", type, reg, critlo, type, reg, crithi);
                }
                printf("\n");
                write_config_file("\n");
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

    if ((node < 0) && (node != -FDT_ERR_NOTFOUND))
    {
        cancel(21, "Error traversing i2c device %d-%04x (%s), node 0x%08x: %s\n", bus, dev, devlabel, parent, fdt_strerror(node));
    }

    close_config_file();
}

static void i2c_traverse(int bus, int offset)
{
    int node;
    fdt_for_each_subnode(node, dtb, offset)
    {
        const char *pcompatible = getprop(node, "compatible", 1, 11, "Error reading compatible value from node 0x%08x:", node);
        for (const char *compatible; (compatible = strchr(pcompatible, ',')) != NULL; pcompatible = compatible + 1);
        const int *preg = getprop(node, "reg", 1, 12, "Error reading reg value from node 0x%08x:", node);
        int reg = *preg >> 24;
        const char *label = getprop(node, "label", 0, 13, "Error reading label value from node 0x%08x:", node);

        printf("Node (0x%08x): compatible=\"%s\", bus=%d, slave=0x%02x, label=\"%s\" found\n", node, pcompatible, bus, reg, label);
        create_config(node, bus, reg, label);
        instantiate(pcompatible, bus, reg);
    }

    if ((node < 0) && (node != -FDT_ERR_NOTFOUND))
    {
        cancel(10, "Error traversing i2c bus at %d: %s\n", offset, fdt_strerror(node));
    }
}

int main(int argc, char *argv[])
{
    atexit(close_config_file);

    const char *fname = check_args(argc, argv, devtree_path);
    long dtb_size;
    if (readfile(fname, &dtb, &dtb_size)) cancel(1, "Can't read DTB file %s\n", fname);
    atexit(free_dtb);

    int bmc_offset = fdt_path_offset(dtb, "/bmc");
    if (bmc_offset < 0) cancel(2, "Can't find /bmc block in DTB: %s\n", fdt_strerror(bmc_offset));
    printf("/bmc block found at offset 0x%08x\n", bmc_offset);

    for (int i2c = 0; i2c < 15; ++i2c)
    {
        char i2cbus[10];
        memset(i2cbus, 0, sizeof(i2cbus));
        snprintf(i2cbus, 9, "i2c%d", i2c);

        const char *path = getprop(bmc_offset, i2cbus, 0, 3, "Error enumerating i2c buses in /bmc block:");
        if (path == empty)
        {
            printf("Bus %s not found in DTB\n", i2cbus);
        }
        else
        {
            printf("Bus %s corresponds to path %s\n", i2cbus, path);
            int i2c_offset = fdt_path_offset(dtb, path);
            if (i2c_offset < 0) cancel(4, "Can't find %s path in DTB: %s\n", path, fdt_strerror(bmc_offset));
            printf("%s (%s) node found at offset 0x%08x\n", i2cbus, path, i2c_offset);
            i2c_traverse(i2c, i2c_offset);
        }
    }

    return 0;
}
