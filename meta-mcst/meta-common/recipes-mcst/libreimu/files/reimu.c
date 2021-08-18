#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <sys/stat.h>

int reimu_is_in_dict(const char *dict[], const char *name)
{
    for (int i = 0; dict[i] != NULL; ++i)
    {
        if (!strcmp(dict[i], name)) return 1;
    }
    return 0;
}

void reimu_set_atexit(int already_done, void (*func)(void))
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
    reimu_set_atexit(reimu_is_atexit_close_config_file, reimu_close_config_file);

    reimu_configfile = fopen(path, "w");
    return (reimu_configfile == NULL);
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
