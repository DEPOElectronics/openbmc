#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <time.h>
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

int reimu_is_atexit_close_text_file = 0;
FILE *reimu_textfile = NULL;

void reimu_close_text_file(void)
{
    if (reimu_textfile) fclose(reimu_textfile);
    reimu_textfile = NULL;
}

int reimu_create_text_file(const char *path)
{
    reimu_set_atexit(reimu_is_atexit_close_text_file, reimu_close_text_file);

    if (reimu_textfile != NULL) return 1;
    reimu_textfile = fopen(path, "w");
    return (reimu_textfile == NULL);
}

void __attribute__((format(printf, 2, 3))) reimu_cancel(int num, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    exit(num);
}

void __attribute__((format(printf, 2, 3))) reimu_message(FILE *stream, const char *fmt, ...)
{
    va_list ap;
    va_start (ap, fmt);
    vfprintf(stream, fmt, ap);
    va_end (ap);
    fflush(stream);
}

char reimu_s_timestr[1024];
char *reimu_gettime(void)
{
    time_t tm = time(NULL);
    strncpy(reimu_s_timestr, ctime(&tm), 1023);

    char *pos;
    for(pos = reimu_s_timestr; (*pos != '\0') && (*pos != '\n'); ++pos);
    *pos = '\0';

    return reimu_s_timestr;
}

void reimu_msleep(long value)
{
    struct timespec rem, req = { value / 1000L, (value % 1000L) * 1000000L };
    while(nanosleep(&req, &rem)) req = rem;
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

void __attribute__((format(printf, 1, 2))) reimu_write_text_file(const char *fmt, ...)
{
    if (!reimu_textfile) reimu_cancel(50, "Error while writing text file: File isn't opened\n");
    va_list ap;
    va_start(ap, fmt);
    int rv = vfprintf(reimu_textfile, fmt, ap);
    va_end(ap);
    if (rv < 0) reimu_cancel(51, "Error while writing text file: vfprintf() returned %d\n", rv);
}
