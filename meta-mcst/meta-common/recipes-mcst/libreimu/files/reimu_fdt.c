#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <libfdt.h>
#include "reimu.h"

#define REIMU_DEBUG 0

const char *reimu_devtree_path = "/var/volatile/motherboard_devtree.dtb";

const char *reimu_prop_empty = "";
int reimu_is_prop_empty(const char *prop)
{
    return (prop == reimu_prop_empty);
}

int reimu_is_atexit_free_dtb = 0;
char *reimu_dtb = NULL;

void reimu_free_dtb(void)
{
    free(reimu_dtb);
}

const void* __attribute__((format(printf, 5, 6))) reimu_getprop(int node, const char *name, int mandatory, int failval, const char *fmt, ...)
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

int reimu_for_each_subnode(int parent, traverse_callback_t callback, const void *data, int bus, void (*function)(int node, const char *nodename, traverse_callback_t callback, const void *data, int bus))
{
    int node;
    fdt_for_each_subnode(node, reimu_dtb, parent)
    {
        const char *nodename = fdt_get_name(reimu_dtb, node, NULL);
        function(node, nodename, callback, data, bus);
    }
    if ((node < 0) && (node != -FDT_ERR_NOTFOUND))
    {
        return 1;
    }
    return 0;
}

int reimu_open_dtb(void)
{
    long dtb_size;
    if (reimu_readfile(reimu_devtree_path, &reimu_dtb, &dtb_size)) return 1;
    reimu_set_atexit(reimu_is_atexit_free_dtb, reimu_free_dtb);
    return 0;
}

void reimu_traverse_node(int node, const char *nodename, traverse_callback_t callback, const void *data, int bus)
{
    const char *pcompatible = reimu_getprop(node, "compatible", 1, 11, "Error reading compatible value from node 0x%08x:", node);
    for (const char *compatible; (compatible = strchr(pcompatible, ',')) != NULL; pcompatible = compatible + 1);
    const int *preg = reimu_getprop(node, "reg", 1, 12, "Error reading reg value from node 0x%08x:", node);
    int reg = *preg >> 24;
    const char *label = reimu_getprop(node, "label", 0, 13, "Error reading label value from node 0x%08x:", node);
    #if REIMU_DEBUG
        printf("Node %s (0x%08x): compatible=\"%s\", bus=%d, slave=0x%02x, label=\"%s\" found\n", nodename, node, pcompatible, bus, reg, label);
    #endif
    callback(pcompatible, node, bus, reg, label, data);
}

int reimu_i2c_traverse(int bus, int offset, const void *data, traverse_callback_t callback)
{
    return reimu_for_each_subnode(offset, callback, data, bus, reimu_traverse_node);
}

void reimu_traverse_all_i2c(const void *data, traverse_callback_t callback)
{
    if (reimu_open_dtb()) reimu_cancel(1, "Can't read device tree file\n");

    int bmc_offset = fdt_path_offset(reimu_dtb, "/bmc");
    if (bmc_offset < 0) reimu_cancel(2, "Can't find /bmc block in DTB: %s\n", fdt_strerror(bmc_offset));
    #if REIMU_DEBUG
        printf("/bmc block found at offset 0x%08x\n", bmc_offset);
    #endif
    for (int i2c = 0; i2c < 15; ++i2c)
    {
        char i2cbus[10];
        memset(i2cbus, 0, sizeof(i2cbus));
        snprintf(i2cbus, 9, "i2c%d", i2c + 1);

        const char *path = reimu_getprop(bmc_offset, i2cbus, 0, 3, "Error enumerating i2c buses in /bmc block:");
        if (reimu_is_prop_empty(path))
        {
            #if REIMU_DEBUG
                printf("Bus %s not found in DTB\n", i2cbus);
            #endif
        }
        else
        {
            #if REIMU_DEBUG
                printf("Bus %s corresponds to path %s\n", i2cbus, path);
            #endif
            int i2c_offset = fdt_path_offset(reimu_dtb, path);
            if (i2c_offset < 0) reimu_cancel(4, "Can't find %s path in DTB: %s\n", path, fdt_strerror(bmc_offset));
            #if REIMU_DEBUG
                printf("%s (%s) node found at offset 0x%08x\n", i2cbus, path, i2c_offset);
            #endif
            if (reimu_i2c_traverse(i2c, i2c_offset, data, callback)) reimu_cancel(10, "Error traversing i2c bus at %d\n", i2c_offset);
        }
    }
}
