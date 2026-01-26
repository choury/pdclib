/* POSIX inet implementations for the bpf platform.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdint.h>

#include <arpa/inet.h>

uint16_t htons(uint16_t hostshort)
{
    return __builtin_bswap16(hostshort);
}

uint16_t ntohs(uint16_t netshort)
{
    return __builtin_bswap16(netshort);
}

uint32_t htonl(uint32_t hostlong)
{
    return __builtin_bswap32(hostlong);
}

uint32_t ntohl(uint32_t netlong)
{
    return __builtin_bswap32(netlong);
}
