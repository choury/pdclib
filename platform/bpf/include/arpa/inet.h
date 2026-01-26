/* arpa/inet.h - Internet definitions

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_ARPA_INET_H
#define _PDCLIB_BPF_ARPA_INET_H

#include "pdclib/_PDCLIB_internal.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

_PDCLIB_PUBLIC uint16_t htons(uint16_t hostshort);
_PDCLIB_PUBLIC uint16_t ntohs(uint16_t netshort);
_PDCLIB_PUBLIC uint32_t htonl(uint32_t hostlong);
_PDCLIB_PUBLIC uint32_t ntohl(uint32_t netlong);

#ifdef __cplusplus
}
#endif

#endif