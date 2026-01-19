/* _PDCLIB_close( _PDCLIB_fd_t )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

/* This is an example implementation of _PDCLIB_close() fit for use with POSIX
   kernels.
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

int _PDCLIB_close( int fd )
{
    int rc = close( fd );

    if ( rc < 0 )
    {
        *_PDCLIB_errno_func() = -rc;
        return -1;
    }

    return 0;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    /* No testdriver; tested in driver for _PDCLIB_open(). */
    return TEST_RESULTS;
}

#endif
