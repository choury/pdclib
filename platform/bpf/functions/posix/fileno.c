/* fileno( FILE * )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdio.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_internal.h"

int fileno( struct _PDCLIB_file_t * stream )
{
    return stream->handle;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    TESTCASE( fileno( stdin ) == 0 );
    TESTCASE( fileno( stdout ) == 1 );
    TESTCASE( fileno( stderr ) == 2 );
    return TEST_RESULTS;
}

#endif
