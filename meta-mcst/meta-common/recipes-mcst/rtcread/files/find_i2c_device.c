#include <stdio.h>
#include <string.h>
#include <reimu.h>
#include <errno.h>

static void check_device(const char *pcompatible, int node, int bus, int reg, const char *label, const void *data)
{
    const char *model = (const char *)data;
    if (!strcmp(pcompatible, model) && !strcmp(label, "platform")) printf("%s:%x:%d\n", model, reg, bus);
}

int main(int argc, char *argv[])
{
    if (argc != 2) reimu_cancel(EINVAL, "You must specify compatible name\n");
    reimu_traverse_all_i2c(argv[1], check_device);
    return 0;
}
