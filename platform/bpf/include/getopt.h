/* getopt.h - Command-line option parsing

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_GETOPT_H
#define _PDCLIB_BPF_GETOPT_H

#include "pdclib/_PDCLIB_internal.h"

#ifdef __cplusplus
extern "C" {
#endif

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;

_PDCLIB_PUBLIC int getopt(int argc, char * const argv[], const char *optstring);

#ifdef __cplusplus
}
#endif

#endif