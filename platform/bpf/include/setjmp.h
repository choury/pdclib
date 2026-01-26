/* setjmp.h - Non-local jumps

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef _PDCLIB_BPF_SETJMP_H
#define _PDCLIB_BPF_SETJMP_H

#include "pdclib/_PDCLIB_internal.h"
#include <stdint.h>
#include "bpf_call.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t regs[16];
} jmp_buf[1];

#define setjmp(env) ((int)syscall(BPF_CALL_SETJMP, (long)(env)))
_PDCLIB_PUBLIC __attribute__((noreturn)) void longjmp(jmp_buf env, int val);

#ifdef __cplusplus
}
#endif

#endif