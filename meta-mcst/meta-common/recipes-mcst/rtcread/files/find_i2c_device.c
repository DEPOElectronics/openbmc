#include <stdio.h>
#include <libfdt.h>
#include <errno.h>

const char *devtree_path = "/var/volatile/motherboard_devtree.dtb";

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

char *dtb;

static void free_dtb(void)
{
    free(dtb);
}

const char *empty = "";

static int find_device(int bus, int offset, const char *model)
{
    int node;
    fdt_for_each_subnode(node, dtb, offset)
    {
        int err;
        const char *pcompatible = fdt_getprop(dtb, node, "compatible", NULL);
        if (pcompatible == NULL) return 0;
        for (const char *compatible; (compatible = strchr(pcompatible, ',')) != NULL; pcompatible = compatible + 1);
        const int *preg = fdt_getprop(dtb, node, "reg", NULL);
        if (preg == NULL) return 0;
        int reg = *preg >> 24;
        if (!strcmp(pcompatible, model)) return reg;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) return EINVAL;

    long dtb_size;
    if (readfile(devtree_path, &dtb, &dtb_size)) return EBADF;
    atexit(free_dtb);

    int bmc_offset = fdt_path_offset(dtb, "/bmc");
    if (bmc_offset < 0) return ENOKEY;

    for (int i2c = 0; i2c < 15; ++i2c)
    {
        char i2cbus[10];
        memset(i2cbus, 0, sizeof(i2cbus));
        snprintf(i2cbus, 9, "i2c%d", i2c + 1);

        const char *path = fdt_getprop(dtb, bmc_offset, i2cbus, NULL);
        if (path != NULL)
        {
            int i2c_offset = fdt_path_offset(dtb, path);
            if (i2c_offset < 0) return ERANGE;
            int slave = find_device(i2c, i2c_offset, argv[1]);
            if(slave != 0)
            {
                printf("%s:%x:%d\n", argv[1], slave, i2c);
                return 0;
            }
        }
    }
    return ENOENT;
}
