/* libgen.h - Pattern matching functions

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_LIBGEN_H
#define _PDCLIB_BPF_LIBGEN_H

#include "pdclib/_PDCLIB_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

_PDCLIB_PUBLIC char *basename(char *path);
_PDCLIB_PUBLIC char *dirname(char *path);

#ifdef __cplusplus
}
#endif

#endif