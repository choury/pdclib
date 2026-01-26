/* pwd.h - Password structure

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_PWD_H
#define _PDCLIB_BPF_PWD_H

#include "pdclib/_PDCLIB_internal.h"
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

struct passwd {
    char   *pw_name;       /* username */
    char   *pw_passwd;     /* user password */
    uid_t   pw_uid;        /* user ID */
    gid_t   pw_gid;        /* group ID */
    char   *pw_gecos;      /* user information */
    char   *pw_dir;        /* home directory */
    char   *pw_shell;      /* shell program */
};

_PDCLIB_PUBLIC struct passwd *getpwuid(uid_t uid);
_PDCLIB_PUBLIC struct passwd *getpwnam(const char *name);

#ifdef __cplusplus
}
#endif

#endif