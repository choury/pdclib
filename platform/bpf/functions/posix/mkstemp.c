/* mkstemp() and mkdtemp() for BPF platform.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#ifndef REGTEST

static int generate_name(char *template)
{
    static const char chars[] =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static unsigned long long state = 0;
    size_t len = strlen(template);
    size_t i;

    if (len < 6) {
        errno = EINVAL;
        return -1;
    }

    for (i = len - 6; i < len; i++) {
        if (template[i] != 'X') {
            errno = EINVAL;
            return -1;
        }
    }

    if (state == 0) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC, &ts);
        state = (unsigned long long)ts.tv_sec * 1000000000ULL +
                (unsigned long long)ts.tv_nsec;
        state ^= (unsigned long long)(long)template;
    }

    for (i = len - 6; i < len; i++) {
        state = state * 6364136223846793005ULL + 1442695040888963407ULL;
        template[i] = chars[(state >> 33) % (sizeof(chars) - 1)];
    }

    return 0;
}

int mkstemp(char *template)
{
    int attempts = 100;
    int fd;

    while (attempts-- > 0) {
        char save[6];
        size_t len = strlen(template);

        if (len < 6) {
            errno = EINVAL;
            return -1;
        }
        memcpy(save, template + len - 6, 6);

        if (generate_name(template) < 0) {
            return -1;
        }

        fd = open(template, O_RDWR | O_CREAT | O_EXCL, 0600);
        if (fd >= 0) {
            return fd;
        }

        if (errno != EEXIST) {
            memcpy(template + len - 6, save, 6);
            return -1;
        }

        memcpy(template + len - 6, save, 6);
    }

    errno = EEXIST;
    return -1;
}

char *mkdtemp(char *template)
{
    int attempts = 100;

    while (attempts-- > 0) {
        char save[6];
        size_t len = strlen(template);

        if (len < 6) {
            errno = EINVAL;
            return NULL;
        }
        memcpy(save, template + len - 6, 6);

        if (generate_name(template) < 0) {
            return NULL;
        }

        if (mkdir(template, 0700) == 0) {
            return template;
        }

        if (errno != EEXIST) {
            memcpy(template + len - 6, save, 6);
            return NULL;
        }

        memcpy(template + len - 6, save, 6);
    }

    errno = EEXIST;
    return NULL;
}

#endif
