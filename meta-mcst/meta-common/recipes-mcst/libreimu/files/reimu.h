#ifndef REIMU_H
#define REIMU_H

/* reimu_string */

int reimu_is_in_dict(const char *dict[], const char *name);

/* reimu_control */

void reimu_cancel(int num, const char *fmt, ...);

/* reimu_files */

int reimu_readfile(const char *name, char **p_buf, long *p_size);
int reimu_writefile(const char *name, const void *buf, long size);

/* reimu_dirs */

int reimu_chkdir(const char *path);
int reimu_recurse_mkdir(char *path);

/* reimu_devicetree */

void reimu_free_dtb(void);
int reimu_open_dtb(void);
const void *reimu_getprop(int node, const char *name, int mandatory, int failval, const char *fmt, ...);
int reimu_is_prop_empty(const char *prop);
int reimu_for_each_subnode(int parent, void *data, void *data2, void (*function)(int node, const char *nodename, void *data, void *data2));

/* reimu_configfile */

int reimu_is_config_file_opened(void);
int reimu_create_config_file(const char *path);
void reimu_close_config_file(void);
void reimu_write_config_file(const char *fmt, ...);

/* reimu_i2c */

void reimu_traverse_all_i2c(void *data, void (*function)(const char *pcompatible, int node, int bus, int reg, const char *label, const void *data));
int reimu_i2c_traverse(int bus, int offset, void *data, void (*function)(const char *pcompatible, int node, int bus, int reg, const char *label, const void *data));

#endif