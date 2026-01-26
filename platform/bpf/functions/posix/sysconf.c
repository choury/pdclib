/* sysconf( int )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <errno.h>
#include <unistd.h>
#include <limits.h>

#ifndef REGTEST

long sysconf( int name )
{
    if ( name == _SC_PAGE_SIZE || name == _SC_PAGESIZE )
    {
        return 4096;
    }

    switch ( name )
    {
        case _SC_CLK_TCK:
            return 100;
        case _SC_ARG_MAX:
            return _POSIX_ARG_MAX;
        case _SC_LOGIN_NAME_MAX:
            return 256;
        default:
            errno = EINVAL;
            return -1;
    }
}

long pathconf( const char * path, int name )
{
    (void)path;
    (void)name;
    errno = EINVAL;
    return -1;
}

long fpathconf( int fd, int name )
{
    (void)fd;
    (void)name;
    errno = EINVAL;
    return -1;
}

size_t confstr( int name, char * buf, size_t len )
{
    (void)name;
    if ( buf != NULL && len > 0 )
    {
        buf[ 0 ] = '\0';
    }
    return 0;
}

#endif
