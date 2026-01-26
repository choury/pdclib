/* time( time_t * )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <time.h>
#include <errno.h>
#include <unistd.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_defguard.h"

#include "sys/time.h"

/* See comments in _PDCLIB_config.h on the semantics of time_t. */

time_t time( time_t * timer )
{
    struct timeval tv;

    if ( gettimeofday( &tv, NULL ) == 0 )
    {
        if ( timer != NULL )
        {
            *timer = tv.tv_sec;
        }

        return tv.tv_sec;
    }

    return -1;
}

unsigned int sleep( unsigned int seconds )
{
    struct timespec req;
    struct timespec rem;

    req.tv_sec = (time_t)seconds;
    req.tv_nsec = 0;

    if ( nanosleep( &req, &rem ) == 0 )
    {
        return 0;
    }

    if ( errno == EINTR )
    {
        unsigned int left = (unsigned int)rem.tv_sec;
        if ( rem.tv_nsec > 0 )
        {
            left += 1;
        }
        return left;
    }

    return seconds;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    time_t t = time( NULL );
    printf( "%d\n", (int)t );
    TESTCASE( NO_TESTDRIVER );
    return TEST_RESULTS;
}

#endif
