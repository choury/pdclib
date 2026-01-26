/* POSIX min setjmp implementations for the bpf platform.

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <setjmp.h>
#include "bpf_call.h"

__attribute__((noreturn)) void longjmp(jmp_buf env, int val)
{
    syscall(BPF_CALL_LONGJMP, (long)env, (long)val);
    while (1);
}
