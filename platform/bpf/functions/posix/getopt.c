/* Minimal getopt implementation for the bpf platform.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <getopt.h>
#include <string.h>

char *optarg;
int optind = 1;
int opterr = 1;
int optopt;

int getopt(int argc, char * const argv[], const char *optstring)
{
    static const char *next = 0;
    const char *arg = 0;
    char c = 0;
    const char *opt = 0;
    if (optind <= 0)
    {
        optind = 1;
        next = 0;
    }

    if (next == 0 || *next == '\0')
    {
        if (optind >= argc)
        {
            return -1;
        }
        arg = argv[optind];
        if (arg == 0 || arg[0] != '-' || arg[1] == '\0')
        {
            return -1;
        }
        if (arg[1] == '-' && arg[2] == '\0')
        {
            optind++;
            return -1;
        }
        next = arg + 1;
        optind++;
    }

    c = *next++;
    opt = strchr(optstring, c);
    if (opt == 0 || c == ':')
    {
        optopt = (int)c;
        return '?';
    }

    if (opt[1] == ':')
    {
        if (*next != '\0')
        {
            optarg = (char *)next;
            next = 0;
        }
        else if (optind < argc)
        {
            optarg = argv[optind++];
            next = 0;
        }
        else
        {
            optopt = (int)c;
            if (optstring[0] == ':')
            {
                return ':';
            }
            return '?';
        }
    }
    else
    {
        optarg = 0;
    }

    return (int)c;
}
