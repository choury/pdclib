/* _PDCLIB_remove( const char * )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

/* This is an example implementation of _PDCLIB_remove() fit for use with
   POSIX kernels.
*/
#include <fcntl.h>
#include <unistd.h>
#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

int _PDCLIB_remove( const char * pathname )
{
    int rc = unlink( pathname );

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
    /* Testing covered by ftell.c (and several others) */
    return TEST_RESULTS;
}

#endif
