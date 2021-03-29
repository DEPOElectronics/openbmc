#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <rfb/rfbproto.h>

static __attribute__((format(printf, 2, 3))) int error(int code, const char *format, ...)
{
    va_list args;
    va_start (args, format);
    vfprintf (stderr, format, args);
    va_end (args);
    exit(code);
}

int main()
{
    const char *fname = "~/.vnc/passwd";
    int pass_max = sysconf(_SC_PASS_MAX);
    if (pass_max < 0) pass_max = 8192;

    char pass1[pass_max + 1], pass2[pass_max + 1],  *pass;
    pass = getpass("Password: ");
    if (!pass) error(1, "Can't read password");
    strncpy(pass1, pass, pass_max);

    pass = getpass("Verify: ");
    if (!pass) error(1, "Can't read password verification");
    strncpy(pass2, pass, pass_max);

    if (strcmp(pass1, pass2)) error(2, "Passwords don't match");

    if (rfbEncryptAndStorePasswd(pass1, fname)) error(3, "Can't save password to %s", fname);
}