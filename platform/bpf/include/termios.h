/* termios.h - Terminal control definitions

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_TERMIOS_H
#define _PDCLIB_BPF_TERMIOS_H

#include "pdclib/_PDCLIB_internal.h"
#include <sys/types.h>
#include_next <termios.h>

#ifdef __cplusplus
extern "C" {
#endif

_PDCLIB_PUBLIC int tcgetattr(int fd, struct termios *termios_p);

#ifdef __cplusplus
}
#endif

#endif