/* wchar.h - Wide character handling

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_WCHAR_H
#define _PDCLIB_BPF_WCHAR_H

#include "pdclib/_PDCLIB_internal.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _PDCLIB_MBSTATE_T_DEFINED
#define _PDCLIB_MBSTATE_T_DEFINED
typedef struct {
    unsigned int __state;
} mbstate_t;
#endif

_PDCLIB_PUBLIC size_t mbrlen(const char *s, size_t n, mbstate_t *ps);
_PDCLIB_PUBLIC size_t mbrtowc(wchar_t *pwc, const char *s, size_t n, mbstate_t *ps);
_PDCLIB_PUBLIC size_t mbsrtowcs(wchar_t *dst, const char **src, size_t len, mbstate_t *ps);
_PDCLIB_PUBLIC wchar_t *wcschr(const wchar_t *s, wchar_t c);

#ifdef __cplusplus
}
#endif

#endif