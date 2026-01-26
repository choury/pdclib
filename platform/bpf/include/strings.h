/* strings.h - String operations

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_STRINGS_H
#define _PDCLIB_BPF_STRINGS_H

#include "pdclib/_PDCLIB_internal.h"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

_PDCLIB_PUBLIC char * stpncpy(char *dst, const char *src, size_t n);
_PDCLIB_PUBLIC char * strdup( const char * s );
_PDCLIB_PUBLIC char * strndup( const char * s, size_t n );

_PDCLIB_PUBLIC int strcasecmp(const char *s1, const char *s2);
_PDCLIB_PUBLIC int strncasecmp(const char *s1, const char *s2, size_t n);

#ifdef __cplusplus
}
#endif

#endif