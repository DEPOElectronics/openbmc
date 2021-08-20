#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "reimu.h"

static int get_power_state(void)
{
    return reimu_get_gpio_by_name("GPIO_POWER_IN");
}

static int press_button(const char *sym, int delay)
{
    return reimu_set_gpio_by_name(sym, 0, delay);
}

static int server_pwrbut_s(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    return press_button("GPIO_PWR_BTN", 200);
}

static int server_pwrbut_h(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    return press_button("GPIO_PWR_BTN", 5000);
}

static int server_reset(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    return press_button("GPIO_RST_BTN", 200);
}

static int server_pwr_on(int argc, char *argv[])
{
    int power_state = get_power_state();
    switch(power_state)
    {
        case 1:  return 0;
        case 0:  return server_pwrbut_s(argc, argv);
        default: return power_state;
    }
}

static int server_pwr_off(int argc, char *argv[])
{
    int power_state = get_power_state();
    switch(power_state)
    {
        case 0:  return 0;
        case 1:  return server_pwrbut_s(argc, argv);
        default: return power_state;
    }
}

static int server_pwr_off_hard(int argc, char *argv[])
{
    int power_state = get_power_state();
    switch(power_state)
    {
        case 0:  return 0;
        case 1:  return server_pwrbut_h(argc, argv);
        default: return power_state;
    }
}

static int dbus_watchdog_off(void)
{
    return reimu_dbus_set_property_bool("xyz.openbmc_project.Watchdog", "/xyz/openbmc_project/watchdog/host0", "xyz.openbmc_project.State.Watchdog", "Enabled", 0);
}

static int server_watchdog_reset(int argc, char *argv[])
{
    /* 10 seconds should be fair enough for power to became on. */
    for(int sec = 10; sec > 0; --sec)
    {
        int power_state = get_power_state();
        switch(power_state)
        {
            case 1: return dbus_watchdog_off();
            case 0: break;
            default: return power_state;
        }
        reimu_msleep(1000);
    }
    return 35;
}

enum { UID_API_NONE, UID_API_DBUS, UID_API_TSPI, UID_API_GPIO } s_uid_api = UID_API_NONE;

static int uid_init(void)
{
    /* Return if already initialized */
    if (s_uid_api != UID_API_NONE) return 0;

    /* Try to introspect LED manager. If success, consider DBUS protocol applicable. */
    if (!reimu_dbus_call_method("xyz.openbmc_project.LED.GroupManager", "/", "org.freedesktop.DBus.Introspectable", "Introspect"))
    {
        s_uid_api = UID_API_DBUS;
        return 0;
    }

    /* Try to check whether TinySPI is enabled, and tinyspi sysfs directory exist. If success, consider TSPI protocol applicable. */
    if (!reimu_find_in_file("/etc/reimu.conf", "TINYSPI=yes"))
    {
        if(reimu_chkdir("/sys/kernel/tinyspi/")) return 39;
        s_uid_api = UID_API_TSPI;
        return 0;
    }

    /* Try to check is LED sysfs directory exist. If success, consider GPIO protocol applicable. */
    if(!reimu_chkdir("/sys/class/leds/platform:blue:uid/"))
    {
        s_uid_api = UID_API_GPIO;
        return 0;
    }

    /* No suitable UID control method found */
    return 40;
}

static int uid_set_dbus(int state)
{
    return reimu_dbus_set_property_bool("xyz.openbmc_project.LED.GroupManager", "/xyz/openbmc_project/led/groups/enclosure_identify", "xyz.openbmc_project.Led.Group", "Asserted", state);
}

static int uid_get_dbus(void)
{
    return reimu_dbus_get_property_bool("xyz.openbmc_project.LED.GroupManager", "/xyz/openbmc_project/led/groups/enclosure_identify", "xyz.openbmc_project.Led.Group", "Asserted");
}

static int uid_set_gpio(int state)
{
    char *trig = state ? "default-on\n" : "none\n";
    return reimu_writefile("/sys/class/leds/platform:blue:uid/trigger", trig, strlen(trig));
}

static int uid_get_gpio(void)
{
    char *led_state = NULL;
    int rv = reimu_readfile("/sys/class/leds/platform:blue:uid/brightness", &led_state, NULL);
    if (rv)
    {
        rv = -1;
    }
    else
    {
        rv = -2;
        if (!strncmp(led_state, "0\n", 2)) rv = 0;
        if (!strncmp(led_state, "255\n", 4)) rv = 1;
    }
    free(led_state);
    return rv;
}

static int uid_set_tspi(int state)
{
    char *bits = state ? "00000004\n" : "00000008\n";
    if(reimu_writefile("/sys/kernel/tinyspi/command_bits_set", bits, strlen(bits))) return 43;
    reimu_msleep(10);
    if(reimu_writefile("/sys/kernel/tinyspi/command_bits_reset", bits, strlen(bits))) return 45;
    return 0;
}

static int uid_get_tspi(void)
{
    char *led_state = NULL;
    int rv = reimu_readfile("/sys/kernel/tinyspi/state_reg", &led_state, NULL);
    if (rv)
    {
        rv = -1;
    }
    else
    {
        rv = -2;
        char *endptr;
        unsigned long res = strtoul(led_state, &endptr, 16);
        if (*endptr == '\n') rv = ((res & 0x00000008) ? 1 : 0);
    }
    free(led_state);
    return rv;
}

static int uid_set(int state)
{
    int rv = uid_init();
    if (rv) return rv;

    switch(s_uid_api)
    {
        case UID_API_DBUS: return uid_set_dbus(state);
        case UID_API_TSPI: return uid_set_tspi(state);
        case UID_API_GPIO: return uid_set_gpio(state);
        default: return 48;
    }
}

static int uid_get(void)
{
    int rv = uid_init();
    if (rv) return rv;

    switch(s_uid_api)
    {
        case UID_API_DBUS: return uid_get_dbus();
        case UID_API_TSPI: return uid_get_tspi();
        case UID_API_GPIO: return uid_get_gpio();
        default: return 49;
    }
}

static int uid_usage(int argc __attribute__((unused)), char *argv[])
{
    printf
    (
        "    Unit ID management utility for REIMU.\n"
        "    Usage:\n"
        "        %s off    - to set UID state off.\n"
        "        %s on     - to set UID state on.\n"
        "        %s switch - to invert current UID state.\n"
        "        %s query  - to query UID state (return code: 0 = off, 1 = on).\n"
        "        %s show   - to do the same as 'query', but also show UID state in terminal.\n",
        argv[0], argv[0], argv[0], argv[0], argv[0]
    );
    return 0;
}

static int uid_on(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    return uid_set(1);
}

static int uid_off(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    return uid_set(0);
}

static int uid_query(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    return uid_get();
}

static int uid_show(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    int uid_state = uid_get();
    switch(uid_state)
    {
        case 0:  printf("UID is OFF\n"); break;
        case 1:  printf("UID is ON\n"); break;
        default: fprintf(stderr, "Can't identify UID state!\n"); break;
    }
    return uid_state;
}

static int uid_switch(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    int uid_state = uid_get();
    switch(uid_state)
    {
        case 0:  return uid_set(1);
        case 1:  return uid_set(0);
        default: fprintf(stderr, "Can't identify UID state!\n"); return uid_state;
    }
}

struct func_list_t { const char *basename; int (*func)(int argc, char *argv[]); };

struct func_list_t uid_cmds[] =
{
    { "on", uid_on },
    { "off", uid_off },
    { "query", uid_query },
    { "show", uid_show },
    { "switch", uid_switch },
    { "-h", uid_usage },
    { "--help", uid_usage }
};

static int server_uid(int argc, char *argv[])
{
    if (argc > 1)
    {
        for (int i = 0; uid_cmds[i].basename != NULL; ++i)
        {
            if (strcmp(argv[1], uid_cmds[i].basename)) continue;
            return uid_cmds[i].func(argc, argv);
        }
        fprintf(stderr, "You should specify a correct command.\n");
        uid_usage(argc, argv);
        return 50;
    }
    return uid_usage(argc, argv);
}

struct func_list_t main_cmds[] =
{
    { "server_pwr_on", server_pwr_on },
    { "server_pwr_off", server_pwr_off },
    { "server_pwr_off_hard", server_pwr_off_hard },
    { "server_pwrbut_s", server_pwrbut_s },
    { "server_pwrbut_h", server_pwrbut_h },
    { "server_reset", server_reset },
    { "server_watchdog_reset", server_watchdog_reset },
    { "server_uid", server_uid },
    { NULL, NULL }
};

int main(int argc, char *argv[])
{
    char *cmd = basename(argv[0]);
    for (int i = 0; main_cmds[i].basename != NULL; ++i)
    {
        if (strcmp(cmd, main_cmds[i].basename)) continue;
        return main_cmds[i].func(argc, argv);
    }
    return 51;
}
