/* Minimal wide-char implementations for the bpf platform.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <string.h>
#include <wchar.h>
#include <wctype.h>

size_t mbrlen(const char *s, size_t n, mbstate_t *ps)
{
    if (ps)
    {
        ps->__state = 0;
    }
    if (s == 0)
    {
        return 0;
    }
    if (n == 0)
    {
        return (size_t)-2;
    }
    if (*s == '\0')
    {
        return 0;
    }
    return 1;
}

size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps)
{
    if (ps)
    {
        ps->__state = 0;
    }
    if (s == 0)
    {
        return 0;
    }
    if (n == 0)
    {
        return (size_t)-2;
    }
    if (*s == '\0')
    {
        if (pwc)
        {
            *pwc = 0;
        }
        return 0;
    }
    if (pwc)
    {
        *pwc = (unsigned char)*s;
    }
    return 1;
}

size_t mbsrtowcs(wchar_t *dst, const char **src, size_t len, mbstate_t *ps)
{
    const char *s = 0;
    size_t count = 0;
    if (ps)
    {
        ps->__state = 0;
    }
    if (src == 0 || *src == 0)
    {
        return 0;
    }

    s = *src;
    if (dst == 0)
    {
        while (*s != '\0')
        {
            ++count;
            ++s;
        }
        return count;
    }

    while (*s != '\0' && count < len)
    {
        dst[count++] = (unsigned char)*s++;
    }

    if (*s == '\0')
    {
        *src = 0;
    }
    else
    {
        *src = s;
    }
    return count;
}

wchar_t *wcschr(const wchar_t *s, wchar_t c)
{
    while (*s)
    {
        if (*s == c)
        {
            return (wchar_t *)s;
        }
        ++s;
    }
    return 0;
}

int iswspace(wint_t wc)
{
    return wc == L' ' || wc == L'\t' || wc == L'\n' ||
           wc == L'\r' || wc == L'\v' || wc == L'\f';
}

wctype_t wctype(const char *property)
{
    if (property && strcmp(property, "space") == 0)
    {
        return (wctype_t)1;
    }
    return (wctype_t)0;
}

int iswctype(wint_t wc, wctype_t desc)
{
    if (desc == (wctype_t)1)
    {
        return iswspace(wc);
    }
    return 0;
}
