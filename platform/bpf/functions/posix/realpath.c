/* realpath( const char * path, char * resolved_name )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef REGTEST

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

char * realpath( const char * path, char * resolved_name )
{
    char buffer[ PATH_MAX ];
    char * dest;
    char * q;
    const char * p;
    size_t len;

    if ( path == NULL || resolved_name == NULL )
    {
        errno = EINVAL;
        return NULL;
    }

    dest = resolved_name;

    if ( path[0] == '/' )
    {
        if ( strlen( path ) >= PATH_MAX )
        {
            errno = ENAMETOOLONG;
            return NULL;
        }
        strcpy( buffer, path );
    }
    else
    {
        if ( getcwd( buffer, PATH_MAX ) == NULL )
        {
            return NULL;
        }
        len = strlen( buffer );
        if ( len + 1 + strlen( path ) >= PATH_MAX )
        {
            errno = ENAMETOOLONG;
            return NULL;
        }
        if ( len > 0 && buffer[len-1] != '/' )
        {
            buffer[len++] = '/';
            buffer[len] = '\0';
        }
        strcpy( buffer + len, path );
    }

    q = dest;
    p = buffer;

    /* Start with root */
    *q++ = '/';
    *q = '\0';

    /* Skip leading slashes in source */
    while ( *p == '/' ) p++;

    while ( *p != '\0' )
    {
        const char * segment_start = p;
        size_t segment_len;

        while ( *p != '\0' && *p != '/' ) p++;
        
        segment_len = p - segment_start;
        
        if ( segment_len == 0 ) 
        {
            /* Multiple slashes, ignore */
        }
        else if ( segment_len == 1 && segment_start[0] == '.' )
        {
            /* . - ignore */
        }
        else if ( segment_len == 2 && segment_start[0] == '.' && segment_start[1] == '.' )
        {
            /* .. - backtrack */
            if ( q > dest + 1 ) 
            {
                q--; /* Skip null */
                while ( q > dest && *q != '/' ) q--;
                if ( q == dest ) q++; /* Don't erase root slash */
                *q = '\0';
            }
        }
        else
        {
            /* Append segment */
            size_t current_len = q - dest;
            if ( current_len + 1 + segment_len >= PATH_MAX )
            {
                errno = ENAMETOOLONG;
                return NULL;
            }
            
            if ( q > dest && q[-1] != '/' )
            {
                *q++ = '/';
            }
            memcpy( q, segment_start, segment_len );
            q += segment_len;
            *q = '\0';
        }

        /* Skip slashes */
        while ( *p == '/' ) p++;
    }

    return dest;
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
