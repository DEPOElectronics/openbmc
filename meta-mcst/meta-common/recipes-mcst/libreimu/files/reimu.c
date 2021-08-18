#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <libfdt.h>
#include <dirent.h>
#include <sys/stat.h>

const char *reimu_devtree_path = "/var/volatile/motherboard_devtree.dtb";

int reimu_is_in_dict(const char *dict[], const char *name)
{
    for (int i = 0; dict[i] != NULL; ++i)
    {
        if (!strcmp(dict[i], name)) return 1;
    }
    return 0;
}

void set_atexit(int already_done, void (*func)(void))
{
    if (already_done) return;
    already_done = 1;
    atexit(func);
}

int reimu_is_atexit_close_config_file = 0;
FILE *reimu_configfile = NULL;

void reimu_close_config_file(void)
{
    if (reimu_configfile) fclose(reimu_configfile);
    reimu_configfile = NULL;
}

int reimu_is_config_file_opened(void)
{
    return (reimu_configfile != NULL);
}

int reimu_create_config_file(const char *path)
{
    set_atexit(reimu_is_atexit_close_config_file, reimu_close_config_file);

    reimu_configfile = fopen(path, "w");
    return (reimu_configfile == NULL);
}

const char *reimu_prop_empty = "";
int reimu_is_prop_empty(const char *prop)
{
    return (prop == reimu_prop_empty);
}

void reimu_cancel(int num, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(num);
}

int reimu_readfile(const char *name, char **p_buf, long *p_size)
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

int reimu_writefile(const char *name, const void *buf, long size)
{
    if(buf == NULL) return 8;

    FILE *f = fopen(name, "w");
    if (f == NULL) return 2;

    if(fwrite(buf, size, 1, f) != 1) { fclose(f); return 6; }
    if(fclose(f)) return 7;

    return 0;
}

int reimu_chkdir(const char *path)
{
    DIR *d;
    if((d = opendir(path)) == NULL) return 1;
    if(closedir(d) != 0) return 2;
    return 0;
}

int reimu_is_atexit_free_dtb = 0;
char *reimu_dtb = NULL;

void reimu_free_dtb(void)
{
    free(reimu_dtb);
}

const void *reimu_getprop(int node, const char *name, int mandatory, int failval, const char *fmt, ...)
{
    int err;
    const void *ptr = fdt_getprop(reimu_dtb, node, name, &err);
    if (ptr == NULL)
    {
        if ((err == -FDT_ERR_NOTFOUND) && !mandatory)
        {
            ptr = reimu_prop_empty;
        }
        else
        {
            va_list ap;
            va_start(ap, fmt);
            vfprintf(stderr, fmt, ap);
            va_end(ap);
            reimu_cancel(failval, " %s\n", fdt_strerror(err));
        }
    }
    return ptr;
}

int reimu_for_each_subnode(int parent, void *data, void *data2, void (*function)(int node, const char *nodename, void *data, void *data2))
{
    int node;
    fdt_for_each_subnode(node, reimu_dtb, parent)
    {
        const char *nodename = fdt_get_name(reimu_dtb, node, NULL);
        function(node, nodename, data, data2);
    }
    if ((node < 0) && (node != -FDT_ERR_NOTFOUND))
    {
        return 1;
    }
    return 0;
}

int reimu_recurse_mkdir(char *path)
{
    if (!reimu_chkdir(path)) return 0;
    for (char *sym = path + 1; *sym; ++sym)
    {
        if (*sym == '/')
        {
            *sym = '\0';
            if (reimu_chkdir(path))
            {
                if(mkdir(path, 0777)) return -1;
            }
            *sym = '/';
        }
    }
    if(mkdir(path, 0777)) return -1;
    return 0;
}

void reimu_write_config_file(const char *fmt, ...)
{
    if (!reimu_configfile) reimu_cancel(50, "Error while writing config file: File isn't opened\n");
    va_list ap;
    va_start(ap, fmt);
    int rv = vfprintf(reimu_configfile, fmt, ap);
    va_end(ap);
    if (rv < 0) reimu_cancel(51, "Error while writing config file: vfprintf() returned %d\n", rv);
}

int reimu_open_dtb(void)
{
    long dtb_size;
    if (reimu_readfile(reimu_devtree_path, &reimu_dtb, &dtb_size)) return 1;
    set_atexit(reimu_is_atexit_free_dtb, reimu_free_dtb);
    return 0;
}

void reimu_traverse_node(int node, const char *nodename, void *data, void *data2)
{
    typedef void (*traverse_callback_t)(const char *, int, int, int, const char *, const void *);
    traverse_callback_t function = (traverse_callback_t) data;
    int bus = *(int*) data2;
    const char *pcompatible = reimu_getprop(node, "compatible", 1, 11, "Error reading compatible value from node 0x%08x:", node);
    for (const char *compatible; (compatible = strchr(pcompatible, ',')) != NULL; pcompatible = compatible + 1);
    const int *preg = reimu_getprop(node, "reg", 1, 12, "Error reading reg value from node 0x%08x:", node);
    int reg = *preg >> 24;
    const char *label = reimu_getprop(node, "label", 0, 13, "Error reading label value from node 0x%08x:", node);
    printf("Node %s (0x%08x): compatible=\"%s\", bus=%d, slave=0x%02x, label=\"%s\" found\n", nodename, node, pcompatible, bus, reg, label);
    function(pcompatible, node, bus, reg, label, data);
}

int reimu_i2c_traverse(int bus, int offset, int op, void (*function)(const char *pcompatible, int node, int bus, int op, int reg, const char *label, const void *data))
{
    return reimu_for_each_subnode(offset, function, &bus, reimu_traverse_node);
}

void reimu_traverse_all_i2c(int op, void (*function)(const char *pcompatible, int node, int bus, int op, int reg, const char *label, const void *data))
{
    int bmc_offset = fdt_path_offset(reimu_dtb, "/bmc");
    if (bmc_offset < 0) reimu_cancel(2, "Can't find /bmc block in DTB: %s\n", fdt_strerror(bmc_offset));
    printf("/bmc block found at offset 0x%08x\n", bmc_offset);

    for (int i2c = 0; i2c < 15; ++i2c)
    {
        char i2cbus[10];
        memset(i2cbus, 0, sizeof(i2cbus));
        snprintf(i2cbus, 9, "i2c%d", i2c + 1);

        const char *path = reimu_getprop(bmc_offset, i2cbus, 0, 3, "Error enumerating i2c buses in /bmc block:");
        if (reimu_is_prop_empty(path))
        {
            printf("Bus %s not found in DTB\n", i2cbus);
        }
        else
        {
            printf("Bus %s corresponds to path %s\n", i2cbus, path);
            int i2c_offset = fdt_path_offset(reimu_dtb, path);
            if (i2c_offset < 0) reimu_cancel(4, "Can't find %s path in DTB: %s\n", path, fdt_strerror(bmc_offset));
            printf("%s (%s) node found at offset 0x%08x\n", i2cbus, path, i2c_offset);
            if (reimu_i2c_traverse(i2c, i2c_offset, op, function)) reimu_cancel(10, "Error traversing i2c bus at %d\n", i2c_offset);
        }
    }
}
