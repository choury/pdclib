/* Termios wrappers for BPF.
   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int tcgetattr(int fd, struct termios *termios_p)
{
    return ioctl(fd, TCGETS, termios_p);
}

int isatty(int fd)
{
    struct termios t;
    return ioctl(fd, TCGETS, &t) == 0;
}

char *ttyname(int fd)
{
    static char name[] = "/dev/tty";

    if (!isatty(fd)) {
        return NULL;
    }

    return name;
}
