#include <gpiod.h>
#include <string.h>
#include "reimu.h"

int reimu_is_atexit_gpiochip_fini = 0;
struct gpiod_chip *reimu_gpiochip = NULL;
void reimu_gpiochip_fini(void)
{
    if (reimu_gpiochip) gpiod_chip_close(reimu_gpiochip);
    reimu_gpiochip = NULL;
}

int reimu_gpiochip_init(void)
{
    if (reimu_gpiochip) return 0;
    reimu_gpiochip = gpiod_chip_open("/dev/gpiochip0");
    reimu_set_atexit(reimu_is_atexit_gpiochip_fini, reimu_gpiochip_fini);
    if (reimu_gpiochip) return 0;
    return 1;
}

int reimu_get_gpio(int num)
{
    int rv = reimu_gpiochip_init();
    if (rv) return -1;

    struct gpiod_line *line = gpiod_chip_get_line(reimu_gpiochip, num);
    if (line == NULL) return -2;

    rv = gpiod_line_request_input(line, "libreimu");
    if (rv) return -3;

    rv = gpiod_line_get_value(line);
    gpiod_line_release(line);
    return (rv < 0) ? -4 : rv;
}

int reimu_set_gpio(int num, int value, int delay)
{
    int rv = reimu_gpiochip_init();
    if (rv) return -1;

    struct gpiod_line *line = gpiod_chip_get_line(reimu_gpiochip, num);
    if (line == NULL) return -2;

    rv = gpiod_line_request_output(line, "libreimu", value);
    if (rv) { gpiod_line_release(line); return -3; }

    reimu_msleep(delay, NULL);
    gpiod_line_release(line);

    rv = gpiod_line_request_input(line, "libreimu");
    gpiod_line_release(line);
    return rv ? -4 : 0;
}

int reimu_is_atexit_gpioconfig_fini = 0;
char *reimu_gpioconfig = NULL;
long reimu_gpioconfig_len = 0;
void reimu_gpioconfig_fini(void)
{
    if (reimu_gpioconfig) free(reimu_gpioconfig);
    reimu_gpioconfig = NULL;
}

int reimu_gpioconfig_init(void)
{
    if (reimu_gpioconfig) return 0;
    int rv = reimu_readfile("/etc/gpiotab", &reimu_gpioconfig, &reimu_gpioconfig_len);
    if (!rv) reimu_set_atexit(reimu_is_atexit_gpioconfig_fini, reimu_gpioconfig_fini);
    return rv;
}

char *reimu_find_gpioconfig(const char *needle)
{
    int ndlen = strlen(needle);
    for(char *pos = reimu_gpioconfig; (pos - reimu_gpioconfig) <= reimu_gpioconfig_len - (ndlen + 3); ++pos)
    {
        if (!strncmp(pos, needle, ndlen) && *(pos + ndlen) == '=')
        {
            char *rv = pos + ndlen + 1;
            if ((rv[0] < 'A') || (rv[0] > 'Z')) return NULL;
            if ((rv[1] >= '0') && (rv[1] <= '9')) return rv;
            if (rv + 2 >= reimu_gpioconfig + reimu_gpioconfig_len) return NULL;
            if ((rv[1] < 'A') || (rv[1] > 'Z')) return NULL;
            if ((rv[2] < '0') || (rv[2] > '9')) return NULL;
            return rv;
        }
    }
    return NULL;
}

int reimu_gpio_to_num(const char *needle)
{
    int rv = reimu_gpioconfig_init();
    if (rv) return -1;
    char *pos = reimu_find_gpioconfig(needle);
    if (pos == NULL) return -2;
    if(pos[1] > '9')
    {
        return (pos[0] - 'A') * 208 + (pos[1] - 'A') * 8 + (pos[2] - '0');
    }
    else
    {
        return (pos[0] - 'A') * 8 + (pos[1] - '0');
    }
}

int reimu_get_gpio_by_name(const char *name)
{
    int rv = reimu_gpio_to_num(name);
    return (rv >= 0) ? reimu_get_gpio(rv) : rv;
}

int reimu_set_gpio_by_name(const char *name, int val, int delay)
{
    int rv = reimu_gpio_to_num(name);
    return (rv >= 0) ? reimu_set_gpio(rv, val, delay) : rv;
}
