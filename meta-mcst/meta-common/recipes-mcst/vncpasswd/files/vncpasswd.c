#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <rfb/rfbproto.h>
#include <reimu.h>

int main(int argc, char *argv[])
{
    char *fname = NULL;
    int pass_max = sysconf(_SC_PASS_MAX);
    if (pass_max < 0) pass_max = 8192;

    if (argc > 1)
    {
        fname = malloc(strlen(argv[1]) + 1);
        if (fname == NULL) reimu_cancel(4, "Out of memory for destination file name\n");
        strcpy(fname, argv[1]);
    }
    else
    {
        const char *homedir;
        if ((homedir = getenv("HOME")) == NULL)
        {
            homedir = getpwuid(getuid())->pw_dir;
            if (strlen(homedir) < 1) reimu_cancel(3, "Can't determine home directory\n");
        }

        const char *default_fname = "/.vnc/passwd";
        fname = malloc(strlen(homedir) + strlen(default_fname) + 1);
        if (fname == NULL) reimu_cancel(4, "Out of memory for destination file name\n");
        strcpy(fname, homedir);
        strcat(fname, default_fname);
    }

    char pass1[pass_max + 1], pass2[pass_max + 1],  *pass;
    pass = getpass("Password: ");
    if (!pass) {free(fname); reimu_cancel(1, "Can't read password\n");}
    strncpy(pass1, pass, pass_max);

    pass = getpass("Verify: ");
    if (!pass) {free(fname); reimu_cancel(1, "Can't read password verification\n");}
    strncpy(pass2, pass, pass_max);

    if (strcmp(pass1, pass2)) {free(fname); reimu_cancel(2, "Passwords don't match\n");}

    int rv;
    if ((rv = rfbEncryptAndStorePasswd(pass1, fname)) != 0) {free(fname); reimu_cancel(rv, "Can't save password to %s (error %d)\n", fname, rv);}
    free(fname);
    return 0;
}
