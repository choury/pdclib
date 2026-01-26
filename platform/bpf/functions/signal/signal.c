/* signal( int, void (*)( int ) )

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <signal.h>

#ifndef REGTEST

#include <stdlib.h>

void ( *_PDCLIB_sigabrt )( int ) = SIG_DFL;
void ( *_PDCLIB_sigfpe )( int )  = SIG_DFL;
void ( *_PDCLIB_sigill )( int )  = SIG_DFL;
void ( *_PDCLIB_sigint )( int )  = SIG_DFL;
void ( *_PDCLIB_sigsegv )( int ) = SIG_DFL;
void ( *_PDCLIB_sigterm )( int ) = SIG_DFL;

const char *const sys_siglist[NSIG] = {
    [0] = "signal 0",
    [SIGHUP] = "SIGHUP",
    [SIGINT] = "SIGINT",
    [SIGQUIT] = "SIGQUIT",
    [SIGILL] = "SIGILL",
    [SIGABRT] = "SIGABRT",
    [SIGFPE] = "SIGFPE",
    [SIGKILL] = "SIGKILL",
    [SIGSEGV] = "SIGSEGV",
    [SIGPIPE] = "SIGPIPE",
    [SIGTERM] = "SIGTERM",
    [SIGCHLD] = "SIGCHLD",
    [SIGCONT] = "SIGCONT",
    [SIGTSTP] = "SIGTSTP",
    [SIGTTIN] = "SIGTTIN",
    [SIGTTOU] = "SIGTTOU",
};

void ( *signal( int sig, void ( *func )( int ) ) )( int )
{
    void ( *oldhandler )( int );

    if ( sig <= 0 || func == SIG_ERR )
    {
        return SIG_ERR;
    }

    switch ( sig )
    {
        case SIGABRT:
            oldhandler = _PDCLIB_sigabrt;
            _PDCLIB_sigabrt = func;
            break;

        case SIGFPE:
            oldhandler = _PDCLIB_sigfpe;
            _PDCLIB_sigfpe = func;
            break;

        case SIGILL:
            oldhandler = _PDCLIB_sigill;
            _PDCLIB_sigill = func;
            break;

        case SIGINT:
            oldhandler = _PDCLIB_sigint;
            _PDCLIB_sigint = func;
            break;

        case SIGSEGV:
            oldhandler = _PDCLIB_sigsegv;
            _PDCLIB_sigsegv = func;
            break;

        case SIGTERM:
            oldhandler = _PDCLIB_sigterm;
            _PDCLIB_sigterm = func;
            break;

        default:
            /* The standard calls for an unspecified "positive value".
               Copying Linux' behaviour of setting EINVAL.
            */
            *_PDCLIB_errno_func() = _PDCLIB_EINVAL;
            return SIG_ERR;
    }

    return oldhandler;
}

static sigset_t process_mask = 0;

int sigemptyset(sigset_t *set)
{
    if (set)
    {
        *set = 0;
    }
    return 0;
}

int sigfillset(sigset_t *set)
{
    if (set)
    {
        *set = (sigset_t)-1;
    }
    return 0;
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
    if (oldset)
    {
        *oldset = process_mask;
    }

    if (set == 0)
    {
        return 0;
    }

    if (how != SIG_SETMASK)
    {
        *_PDCLIB_errno_func() = _PDCLIB_EINVAL;
        return -1;
    }

    process_mask = *set;
    return 0;
}

int sigsetmask(int mask)
{
    sigset_t old = process_mask;
    process_mask = (sigset_t)mask;
    return (int)old;
}

int sigsuspend(const sigset_t *set)
{
    sigset_t old = process_mask;

    if (set)
    {
        process_mask = *set;
    }

    /* No real signal support; report an interrupt and restore mask. */
    process_mask = old;
    *_PDCLIB_errno_func() = _PDCLIB_EINTR;
    return -1;
}

#endif

#ifdef TEST

#include "_PDCLIB_test.h"

int main( void )
{
    /* Testing covered by raise.c */
    return TEST_RESULTS;
}

#endif
