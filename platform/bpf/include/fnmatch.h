/* fnmatch.h - Filename matching

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_FNMATCH_H
#define _PDCLIB_BPF_FNMATCH_H

#include "pdclib/_PDCLIB_internal.h"

#define FNM_NOMATCH     1       /* Match failed. */
#define FNM_NOSYS       2       /* Function not supported. */

#define FNM_NOESCAPE    0x01    /* Disable backslash escaping. */
#define FNM_PATHNAME    0x02    /* Slash must be matched by slash. */
#define FNM_PERIOD      0x04    /* Period must be matched by period. */
#define FNM_LEADING_DIR 0x08    /* Ignore /<any> after a match. */
#define FNM_CASEFOLD    0x10    /* Case insensitive search. */

#ifdef __cplusplus
extern "C" {
#endif

_PDCLIB_PUBLIC int fnmatch(const char *pattern, const char *string, int flags);

#ifdef __cplusplus
}
#endif

#endif /* _PDCLIB_BPF_FNMATCH_H */