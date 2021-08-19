#ifndef REIMU_H
#define REIMU_H

/* reimu_string */

int reimu_is_in_dict(const char *dict[], const char *name);

/* reimu_control */

void reimu_set_atexit(int already_done, void (*func)(void));

/* reimu_logging */

#include <stdio.h>
void __attribute__((format(printf, 2, 3))) reimu_cancel(int num, const char *fmt, ...);
void __attribute__((format(printf, 2, 3))) reimu_message(FILE *stream, const char *fmt, ...);

/* reimu_time */

char *reimu_gettime(void);
void reimu_msleep(long value);

/* reimu_dir */

int reimu_chkdir(const char *path);
int reimu_recurse_mkdir(char *path);

/* reimu_file */

int reimu_readfile(const char *name, char **p_buf, long *p_size);
int reimu_writefile(const char *name, const void *buf, long size);

/* reimu_textfile */

int reimu_create_text_file(const char *path);
void __attribute__((format(printf, 1, 2))) reimu_write_text_file(const char *fmt, ...);
void reimu_close_text_file(void);

/* reimu_gpio (requires libgpiod) */

int reimu_get_gpio_by_name(const char *name);

/* reimu_fdt (requires libfdt) */

typedef void (*traverse_callback_t)(const char *, int, int, int, const char *, const void *);
const void* __attribute__((format(printf, 5, 6))) reimu_getprop(int node, const char *name, int mandatory, int failval, const char *fmt, ...);
int reimu_is_prop_empty(const char *prop);
int reimu_for_each_subnode(int parent, traverse_callback_t callback, const void *data, int bus, void (*function)(int node, const char *nodename,  traverse_callback_t callback, const void *data, int bus));
void reimu_traverse_all_i2c(const void *data, void (*function)(const char *pcompatible, int node, int bus, int reg, const char *label, const void *data));

/* reimu_dbus (requires libdbus-1) */

void reimu_dbus_manage_service(const char *service, const char *command);
int reimu_dbus_set_property_str(const char *service, const char *object, const char *interface, const char *property, const char *value);

/* libreimu internals (to be removed) */

// void reimu_dbus_fini(void);
// void reimu_dbus_msg_fini(void);
// void reimu_init_dbus(void);
// void reimu_free_dtb(void);
// int reimu_open_dtb(void);
// int reimu_i2c_traverse(int bus, int offset, const void *data, void (*function)(const char *pcompatible, int node, int bus, int reg, const char *label, const void *data));

#endif