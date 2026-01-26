/* grp.h - Group database operations

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_GRP_H
#define _PDCLIB_BPF_GRP_H

#include "pdclib/_PDCLIB_internal.h"
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct group {
    char   *gr_name;       /* group name */
    char   *gr_passwd;     /* group password */
    gid_t   gr_gid;        /* group ID */
    char  **gr_mem;        /* group members */
};

_PDCLIB_PUBLIC struct group *getgrgid(gid_t gid);
_PDCLIB_PUBLIC struct group *getgrnam(const char *name);

#ifdef __cplusplus
}
#endif

#endif