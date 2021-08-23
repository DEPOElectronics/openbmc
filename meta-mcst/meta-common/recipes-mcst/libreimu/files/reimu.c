#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
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

int reimu_is_atexit_textfile_close = 0;
FILE *reimu_textfile = NULL;

void reimu_textfile_close(void)
{
    if (reimu_textfile) fclose(reimu_textfile);
    reimu_textfile = NULL;
}

int reimu_textfile_create(const char *name)
{
    reimu_set_atexit(reimu_is_atexit_textfile_close, reimu_textfile_close);

    if (reimu_textfile != NULL) return 1;
    reimu_textfile = fopen(name, "w");
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

char reimu_timestr[1024];
char *reimu_gettime(void)
{
    time_t tm = time(NULL);
    strncpy(reimu_timestr, ctime(&tm), 1023);

    char *pos;
    for(pos = reimu_timestr; (*pos != '\0') && (*pos != '\n'); ++pos);
    *pos = '\0';

    return reimu_timestr;
}

void reimu_msleep(long value, volatile int *exit_request)
{
    struct timespec rem, req = { value / 1000L, (value % 1000L) * 1000000L };
    while(nanosleep(&req, &rem))
    {
        if(exit_request != NULL && *exit_request) exit(0);
        req = rem;
    }
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

int reimu_find_in_file(const char *name, const char *needle)
{
    char *filedata = NULL;
    long filelength = 0;
    if(reimu_readfile(name, &filedata, &filelength)) return -2;

    int rv = -1;
    for(char *pos = filedata; (pos - filedata) <= filelength - strlen(needle); ++pos)
    {
        if (!strncmp(pos, needle, strlen(needle))) { rv = 0; break; }
    }
    free(filedata);
    return rv;
}

int reimu_chkfile(const char *name)
{
    struct stat st;
    if(stat(name, &st)) return -ENOENT;
    if(!S_ISREG(st.st_mode) && !S_ISLNK(st.st_mode)) return -EINVAL;
    return 0;
}

int reimu_chkdir(const char *path)
{
    DIR *d;
    if((d = opendir(path)) == NULL) return -EIO;
    if(closedir(d) != 0) return -EBADF;
    return 0;
}

int reimu_find_filename(int order, char *path, char **name)
{
    DIR *d;
    if((d = opendir(path)) == NULL) return -EIO;

    struct dirent *f;
    while(order >= 0)
    {
        if((f = readdir(d)) == NULL) { closedir(d); return -ENOENT; }
        if (f->d_name[0] != '.') --order;
    }

    if(closedir(d) != 0) return -EBADF;
    if((*name = strdup(f->d_name)) == NULL) return -ENOMEM;
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

void __attribute__((format(printf, 1, 2))) reimu_textfile_write(const char *fmt, ...)
{
    if (!reimu_textfile) reimu_cancel(50, "Error while writing text file: File isn't opened\n");
    va_list ap;
    va_start(ap, fmt);
    int rv = vfprintf(reimu_textfile, fmt, ap);
    va_end(ap);
    if (rv < 0) reimu_cancel(51, "Error while writing text file: vfprintf() returned %d\n", rv);
}

char *reimu_textfile_buf = NULL;

void reimu_textfile_buf_alloc(void)
{
    if ((reimu_textfile_buf = realloc(reimu_textfile_buf, 2)) == NULL) reimu_cancel(94, "Out of memory\n");
    strcpy(reimu_textfile_buf,"");
}

int reimu_textfile_buf_commit(const char *name)
{
    int rv = reimu_writefile(name, reimu_textfile_buf, strlen(reimu_textfile_buf));
    reimu_textfile_buf_alloc();
    return rv;
}

int __attribute__((format(printf, 1, 2))) reimu_textfile_buf_append(const char *fmt, ...)
{
    char str[1024];
    va_list ap;
    va_start (ap, fmt);
    vsnprintf (str, 1023, fmt, ap);
    va_end (ap);
    if ((reimu_textfile_buf = realloc(reimu_textfile_buf, strlen(reimu_textfile_buf) + strlen(str) + 1)) == NULL) reimu_cancel(93, "Out of memory\n");
    strcat(reimu_textfile_buf, str);
}

int reimu_get_conf_bool(const char *confbuf, int conflen, const char *needle)
{
    int ndlen = strlen(needle);
    for(const char *pos = confbuf; (pos - confbuf) <= conflen - ndlen - 2; ++pos)
    {
        if (!strncmp(pos, needle, ndlen))
        {
            if (pos[ndlen] != '=') continue;

            pos += ndlen + 1;
            int toeol = conflen - (pos - confbuf);
            if ((toeol >= 4) && !strncmp(pos, "yes\n", 4)) return 1;
            if ((toeol >= 3) && !strncmp(pos, "no\n",  3)) return 0;
            if ((toeol >= 3) && !strncmp(pos, "on\n",  3)) return 1;
            if ((toeol >= 4) && !strncmp(pos, "off\n", 4)) return 0;
            return -2; /* Malformed data */
        }
    }
    return -1; /* Not found */
}

long reimu_get_conf_long(const char *confbuf, int conflen, const char *needle)
{
    int ndlen = strlen(needle);
    for(const char *pos = confbuf; (pos - confbuf) <= conflen - ndlen - 2; ++pos)
    {
        if (!strncmp(pos, needle, ndlen))
        {
            if (pos[ndlen] != '=') continue;

            pos += ndlen + 1;

            char *endptr;
            long rv = strtol(pos, &endptr, 0);
            if (*endptr != '\n') return -2; /* Malformed data */
            return rv;
        }
    }
    return -1; /* Not found */
}
