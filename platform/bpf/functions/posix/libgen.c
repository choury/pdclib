/* libgen basename/dirname for the bpf platform.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <libgen.h>
#include <string.h>
#include <limits.h>

static char dot[] = ".";
static char slash[] = "/";
static char bname_buffer[PATH_MAX];
static char dname_buffer[PATH_MAX];

char *basename(char *path)
{
    char *end, *start;
    size_t len;

    if (path == NULL || *path == '\0')
    {
        return dot;
    }

    len = strlen(path);
    end = path + len - 1;

    /* Skip trailing slashes */
    while (end > path && *end == '/')
    {
        end--;
    }

    /* All slashes? */
    if (end == path && *end == '/')
    {
        return slash;
    }

    /* Find start of component */
    start = end;
    while (start > path && *(start - 1) != '/')
    {
        start--;
    }

    /* Length of component */
    len = (size_t)(end - start + 1);
    if (len >= PATH_MAX)
    {
        len = PATH_MAX - 1;
    }

    memcpy(bname_buffer, start, len);
    bname_buffer[len] = '\0';

    return bname_buffer;
}

char *dirname(char *path)
{
    char *end, *slash_ptr, *p;
    size_t len;

    if (path == NULL || *path == '\0')
    {
        return dot;
    }

    len = strlen(path);
    end = path + len - 1;

    /* Skip trailing slashes */
    while (end > path && *end == '/')
    {
        end--;
    }

    /* All slashes? */
    if (end == path && *end == '/')
    {
        return slash;
    }

    /* Scan backwards for next slash */
    slash_ptr = end;
    while (slash_ptr > path && *slash_ptr != '/')
    {
        slash_ptr--;
    }

    /* No slash found */
    if (*slash_ptr != '/')
    {
        return dot;
    }

    /* Found slash */
    if (slash_ptr == path)
    {
        /* The slash is at the start (e.g. /usr) */
        return slash;
    }

    /* Remove trailing slashes of parent directory */
    p = slash_ptr;
    while (p > path && *(p - 1) == '/')
    {
        p--;
    }

    len = (size_t)(p - path);
    if (len == 0)
    {
        return slash;
    }

    if (len >= PATH_MAX)
    {
        len = PATH_MAX - 1;
    }

    memcpy(dname_buffer, path, len);
    dname_buffer[len] = '\0';

    return dname_buffer;
}
