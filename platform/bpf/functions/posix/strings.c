/* Extra string functions for the bpf platform.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>


char *stpncpy(char *dst, const char *src, size_t n)
{
    size_t i = 0;
    for (; i < n && src[i] != '\0'; i++)
    {
        dst[i] = src[i];
    }
    if (i < n)
    {
        char *end = dst + i;
        dst[i++] = '\0';
        for (; i < n; i++)
        {
            dst[i] = '\0';
        }
        return end;
    }
    return dst + n;
}

char *strdup(const char *s)
{
    size_t n = 0;
    char *p = NULL;
    while (s[n] != '\0')
    {
        n++;
    }
    p = (char *)malloc(n + 1);
    if (!p)
    {
        return 0;
    }
    for (size_t i = 0; i <= n; i++)
    {
        p[i] = s[i];
    }
    return p;
}

char *strndup(const char *s, size_t n)
{
    size_t len = 0;
    char *p = NULL;
    while (len < n && s[len] != '\0')
    {
        len++;
    }
    p = (char *)malloc(len + 1);
    if (!p)
    {
        return 0;
    }
    for (size_t i = 0; i < len; i++)
    {
        p[i] = s[i];
    }
    p[len] = '\0';
    return p;
}

int strcasecmp(const char *a, const char *b)
{
    unsigned char ca;
    unsigned char cb;
    while (*a && *b)
    {
        ca = (unsigned char)*a;
        cb = (unsigned char)*b;
        if (ca >= 'A' && ca <= 'Z')
        {
            ca = (unsigned char)(ca - 'A' + 'a');
        }
        if (cb >= 'A' && cb <= 'Z')
        {
            cb = (unsigned char)(cb - 'A' + 'a');
        }
        if (ca != cb)
        {
            return (int)ca - (int)cb;
        }
        a++;
        b++;
    }
    return (unsigned char)*a - (unsigned char)*b;
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
    unsigned char c1;
    unsigned char c2;

    if (n == 0) return 0;

    while (n-- > 0)
    {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;

        if (c1 >= 'A' && c1 <= 'Z')
        {
            c1 = (unsigned char)(c1 - 'A' + 'a');
        }
        if (c2 >= 'A' && c2 <= 'Z')
        {
            c2 = (unsigned char)(c2 - 'A' + 'a');
        }

        if (c1 != c2)
        {
            return (int)c1 - (int)c2;
        }
        if (c1 == '\0')
        {
            break;
        }
    }
    return 0;
}