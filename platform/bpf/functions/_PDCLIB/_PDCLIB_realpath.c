/* _PDCLIB_realpath( const char * path )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef REGTEST

#include "pdclib/_PDCLIB_glue.h"

#include <stddef.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

char * _PDCLIB_realpath( const char * path )
{
    char* repath = malloc( PATH_MAX );
    ssize_t len = readlink( path, repath, PATH_MAX - 1 );

    if ( len < 0 )
    {
        /* Not a symlink or other error. Return the path as-is. */
        strncpy( repath, path, PATH_MAX );
        repath[ PATH_MAX - 1 ] = '\0';
        return repath;
    }

    repath[ len ] = '\0';
    return repath;
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
