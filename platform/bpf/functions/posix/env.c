/* setenv(), unsetenv(), putenv()

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <errno.h>
#include <stdlib.h>
#include <string.h>

#ifndef REGTEST

extern char ** environ;

static int env_name_match( const char * entry, const char * name, size_t name_len )
{
    return strncmp( entry, name, name_len ) == 0 && entry[ name_len ] == '=';
}

int setenv( const char * name, const char * value, int overwrite )
{
    size_t name_len;
    size_t value_len;
    size_t index = 0;
    char * entry = NULL;
    char ** new_env = NULL;

    if ( name == NULL || *name == '\0' || strchr( name, '=' ) != NULL )
    {
        errno = EINVAL;
        return -1;
    }

    if ( value == NULL )
    {
        value = "";
    }

    name_len = strlen( name );
    value_len = strlen( value );

    if ( environ != NULL )
    {
        while ( environ[ index ] != NULL )
        {
            if ( env_name_match( environ[ index ], name, name_len ) )
            {
                if ( ! overwrite )
                {
                    return 0;
                }

                entry = malloc( name_len + 1 + value_len + 1 );
                if ( entry == NULL )
                {
                    errno = ENOMEM;
                    return -1;
                }

                memcpy( entry, name, name_len );
                entry[ name_len ] = '=';
                memcpy( entry + name_len + 1, value, value_len + 1 );
                environ[ index ] = entry;
                return 0;
            }
            index++;
        }
    }

    new_env = realloc( environ, ( index + 2 ) * sizeof( char * ) );
    if ( new_env == NULL )
    {
        errno = ENOMEM;
        return -1;
    }
    environ = new_env;

    entry = malloc( name_len + 1 + value_len + 1 );
    if ( entry == NULL )
    {
        errno = ENOMEM;
        return -1;
    }

    memcpy( entry, name, name_len );
    entry[ name_len ] = '=';
    memcpy( entry + name_len + 1, value, value_len + 1 );

    environ[ index ] = entry;
    environ[ index + 1 ] = NULL;
    return 0;
}

int unsetenv( const char * name )
{
    size_t name_len;
    size_t index = 0;
    size_t write_index = 0;

    if ( name == NULL || *name == '\0' || strchr( name, '=' ) != NULL )
    {
        errno = EINVAL;
        return -1;
    }

    name_len = strlen( name );

    if ( environ == NULL )
    {
        return 0;
    }

    while ( environ[ index ] != NULL )
    {
        if ( env_name_match( environ[ index ], name, name_len ) )
        {
            index++;
            continue;
        }

        environ[ write_index ] = environ[ index ];
        write_index++;
        index++;
    }

    environ[ write_index ] = NULL;
    return 0;
}

int putenv( char * string )
{
    char * equals;
    size_t name_len;
    size_t index = 0;
    char ** new_env = NULL;

    if ( string == NULL )
    {
        errno = EINVAL;
        return -1;
    }

    equals = strchr( string, '=' );
    if ( equals == NULL || equals == string )
    {
        errno = EINVAL;
        return -1;
    }

    name_len = (size_t)( equals - string );

    if ( environ != NULL )
    {
        while ( environ[ index ] != NULL )
        {
            if ( env_name_match( environ[ index ], string, name_len ) )
            {
                environ[ index ] = string;
                return 0;
            }
            index++;
        }
    }

    new_env = realloc( environ, ( index + 2 ) * sizeof( char * ) );
    if ( new_env == NULL )
    {
        errno = ENOMEM;
        return -1;
    }
    environ = new_env;

    environ[ index ] = string;
    environ[ index + 1 ] = NULL;
    return 0;
}

#endif
