#include <stdio.h>
#include <string.h>
#include <reimu.h>
#include <errno.h>

static int check_device(enum cancel_type_t unused __attribute((unused)), const char *pcompatible, int node, int bus, int reg, const char *label, const void *data)
{
    const char *model = (const char *)data;
    if (!strcmp(pcompatible, model) && !strcmp(label, RTC_LABEL)) printf("%s:%x:%d\n", model, reg, bus);
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) reimu_cancel(EINVAL, "You must specify compatible name\n");
    reimu_traverse_all_i2c(argv[1], check_device, CANCEL_ON_ERROR);
    return 0;
}
