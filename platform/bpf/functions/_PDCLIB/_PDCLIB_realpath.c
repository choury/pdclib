/* _PDCLIB_realpath( const char * path )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

#include <stdlib.h>
#include <limits.h>

char * _PDCLIB_realpath( const char * path )
{
    char * buffer = malloc( PATH_MAX );
    if ( buffer == NULL ) return NULL;
    if ( realpath( path, buffer ) == NULL )
    {
        free( buffer );
        return NULL;
    }
    return buffer;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    /* No test drivers. */
    return TEST_RESULTS;
}

#endif