/* strptime( const char * restrict, const char * restrict, struct tm * restrict )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <time.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#ifndef REGTEST

#include "pdclib/_PDCLIB_internal.h"

static int _PDCLIB_is_leap_year( int year )
{
    if ( ( year % 4 ) != 0 ) return 0;
    if ( ( year % 100 ) != 0 ) return 1;
    return ( year % 400 ) == 0;
}

static int _PDCLIB_day_of_week( int year, int month, int day )
{
    static const int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    if ( month < 2 ) year -= 1;
    return ( year + year / 4 - year / 100 + year / 400 + t[ month ] + day ) % 7;
}

static int _PDCLIB_week_to_yday( int year, int week, int wday, int week_is_monday )
{
    int jan1_wday = _PDCLIB_day_of_week( year, 0, 1 );
    int first_wday = week_is_monday ? 1 : 0;
    int first_weekday_yday = ( ( first_wday - jan1_wday ) + 7 ) % 7;

    if ( week == 0 )
    {
        int yday = ( ( wday - jan1_wday ) + 7 ) % 7;
        if ( yday >= first_weekday_yday )
        {
            return -1;
        }
        return yday;
    }

    return first_weekday_yday + ( week - 1 ) * 7 + ( ( wday - first_wday + 7 ) % 7 );
}

static const char * _PDCLIB_strptime_int( const char * s, int * out, int min, int max, int width )
{
    int val = 0;
    int digits = 0;

    /* Handle leading zeros and width */
    while ( digits < width && isdigit( (unsigned char)*s ) )
    {
        val = val * 10 + ( *s - '0' );
        s++;
        digits++;
    }
    
    if ( digits == 0 ) return _PDCLIB_NULL;
    if ( val < min || val > max ) return _PDCLIB_NULL;
    *out = val;
    return s;
}

char * strptime( const char * _PDCLIB_restrict s, const char * _PDCLIB_restrict format, struct tm * _PDCLIB_restrict tm )
{
    int century = -1;
    int year_2digit = -1;
    int week_num = -1;
    int week_is_monday = 0;
    int have_year = 0;
    int have_wday = 0;
    int have_yday = 0;
    int year_from_Y = 0;
    int year_from_yy = 0;

    while ( *format != '\0' )
    {
        if ( isspace( (unsigned char)*format ) )
        {
            while ( isspace( (unsigned char)*s ) )
            {
                s++;
            }
            format++;
        }
        else if ( *format != '%' )
        {
            if ( *s != *format )
            {
                return _PDCLIB_NULL;
            }
            s++;
            format++;
        }
        else
        {
            char c;
            format++;
            c = *format++;
            
            if ( c == 'E' || c == 'O' )
            {
                c = *format++;
            }

            switch ( c )
            {
                case 'a':
                case 'A':
                {
                    int i;
                    size_t len;
                    bool found = false;
                    for ( i = 0; i < 7; ++i )
                    {
                        len = strlen( _PDCLIB_lc_time->day_name_full[i] );
                        if ( strncasecmp( s, _PDCLIB_lc_time->day_name_full[i], len ) == 0 )
                        {
                             tm->tm_wday = i;
                             have_wday = 1;
                             s += len;
                             found = true;
                             break;
                        }
                        len = strlen( _PDCLIB_lc_time->day_name_abbr[i] );
                        if ( strncasecmp( s, _PDCLIB_lc_time->day_name_abbr[i], len ) == 0 )
                        {
                             tm->tm_wday = i;
                             have_wday = 1;
                             s += len;
                             found = true;
                             break;
                        }
                    }
                    if ( ! found ) return _PDCLIB_NULL;
                    break;
                }
                case 'b':
                case 'B':
                case 'h':
                {
                    int i;
                    size_t len;
                    bool found = false;
                    for ( i = 0; i < 12; ++i )
                    {
                        len = strlen( _PDCLIB_lc_time->month_name_full[i] );
                        if ( strncasecmp( s, _PDCLIB_lc_time->month_name_full[i], len ) == 0 )
                        {
                             tm->tm_mon = i;
                             s += len;
                             found = true;
                             break;
                        }
                        len = strlen( _PDCLIB_lc_time->month_name_abbr[i] );
                        if ( strncasecmp( s, _PDCLIB_lc_time->month_name_abbr[i], len ) == 0 )
                        {
                             tm->tm_mon = i;
                             s += len;
                             found = true;
                             break;
                        }
                    }
                    if ( ! found ) return _PDCLIB_NULL;
                    break;
                }
                case 'c':
                {
                    s = strptime( s, _PDCLIB_lc_time->date_time_format, tm );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    break;
                }
                case 'C':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 99, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    century = val * 100;
                    if ( year_from_yy && ! year_from_Y )
                    {
                        tm->tm_year = century + year_2digit - 1900;
                    }
                    break;
                }
                case 'd':
                case 'e':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 1, 31, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_mday = val;
                    break;
                }
                case 'D':
                {
                     s = strptime( s, "%m/%d/%y", tm );
                     if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                     break;
                }
                case 'H':
                case 'k':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 23, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_hour = val;
                    break;
                }
                case 'I':
                case 'l':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 1, 12, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_hour = val; 
                    if ( tm->tm_hour == 12 ) tm->tm_hour = 0;
                    break;
                }
                case 'j':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 1, 366, 3 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_yday = val - 1;
                    have_yday = 1;
                    break;
                }
                case 'm':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 1, 12, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_mon = val - 1;
                    break;
                }
                case 'M':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 59, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_min = val;
                    break;
                }
                case 'n':
                case 't':
                {
                    while ( isspace( (unsigned char)*s ) ) s++;
                    break;
                }
                case 'p':
                {
                    size_t len_am = strlen( _PDCLIB_lc_time->am_pm[0] );
                    size_t len_pm = strlen( _PDCLIB_lc_time->am_pm[1] );
                    if ( strncasecmp( s, _PDCLIB_lc_time->am_pm[0], len_am ) == 0 )
                    {
                        if ( tm->tm_hour == 12 ) tm->tm_hour = 0; // 12 AM -> 0
                        s += len_am;
                    }
                    else if ( strncasecmp( s, _PDCLIB_lc_time->am_pm[1], len_pm ) == 0 )
                    {
                        if ( tm->tm_hour < 12 ) tm->tm_hour += 12; // 1 PM -> 13
                        s += len_pm;
                    }
                    else
                    {
                        return _PDCLIB_NULL;
                    }
                    break;
                }
                case 'r':
                {
                    s = strptime( s, _PDCLIB_lc_time->time_format_12h, tm );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    break;
                }
                case 'R':
                {
                    s = strptime( s, "%H:%M", tm );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    break;
                }
                case 'S':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 60, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_sec = val;
                    break;
                }
                case 'T':
                {
                    s = strptime( s, "%H:%M:%S", tm );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    break;
                }
                case 'U':
                case 'W':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 53, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    week_num = val;
                    week_is_monday = ( c == 'W' );
                    break;
                }
                case 'w':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 6, 1 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_wday = val;
                    have_wday = 1;
                    break;
                }
                case 'x':
                {
                    s = strptime( s, _PDCLIB_lc_time->date_format, tm );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    break;
                }
                case 'X':
                {
                    s = strptime( s, _PDCLIB_lc_time->time_format, tm );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    break;
                }
                case 'y':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 99, 2 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    if ( val <= 68 ) val += 100;
                    year_2digit = val;
                    tm->tm_year = val;
                    if ( century >= 0 && ! year_from_Y )
                    {
                        tm->tm_year = century + year_2digit - 1900;
                    }
                    have_year = 1;
                    year_from_yy = 1;
                    break;
                }
                case 'Y':
                {
                    int val;
                    s = _PDCLIB_strptime_int( s, &val, 0, 9999, 4 );
                    if ( s == _PDCLIB_NULL ) return _PDCLIB_NULL;
                    tm->tm_year = val - 1900;
                    century = ( val / 100 ) * 100;
                    year_2digit = val % 100;
                    have_year = 1;
                    year_from_Y = 1;
                    break;
                }
                case '%':
                {
                    if ( *s != '%' ) return _PDCLIB_NULL;
                    s++;
                    break;
                }
                default:
                    return _PDCLIB_NULL;
            }
        }
    }

    if ( week_num >= 0 && have_wday && have_year && ! have_yday )
    {
        int year = tm->tm_year + 1900;
        int yday = _PDCLIB_week_to_yday( year, week_num, tm->tm_wday, week_is_monday );
        int yday_max = _PDCLIB_is_leap_year( year ) ? 366 : 365;
        if ( yday < 0 || yday >= yday_max )
        {
            return _PDCLIB_NULL;
        }
        tm->tm_yday = yday;
    }

    return (char *)s;
}

#endif

#ifdef TEST
#include "_PDCLIB_test.h"

int main( void )
{
    struct tm tm;
    const char * s;

    memset( &tm, 0, sizeof( tm ) );
    s = strptime( "2023-10-27", "%Y-%m-%d", &tm );
    TESTCASE( s != NULL );
    TESTCASE( tm.tm_year == 123 );
    TESTCASE( tm.tm_mon == 9 );
    TESTCASE( tm.tm_mday == 27 );
    
    memset( &tm, 0, sizeof( tm ) );
    s = strptime( "14:30:59", "%H:%M:%S", &tm );
    TESTCASE( s != NULL );
    TESTCASE( tm.tm_hour == 14 );
    TESTCASE( tm.tm_min == 30 );
    TESTCASE( tm.tm_sec == 59 );

    memset( &tm, 0, sizeof( tm ) );
    s = strptime( "20231027", "%Y%m%d", &tm );
    TESTCASE( s != NULL );
    TESTCASE( tm.tm_year == 123 );
    TESTCASE( tm.tm_mon == 9 );
    TESTCASE( tm.tm_mday == 27 );

    return TEST_RESULTS;
}

#endif
