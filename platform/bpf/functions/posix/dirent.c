/* Directory stream operations (opendir / readdir / closedir).

   Implemented entirely in the client on top of the getdents64 syscall,
   which the VM still serves. This file is part of the Public Domain C
   Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#include "bpf_call.h"
#include "pdclib/_PDCLIB_config.h"
#include "pdclib/_PDCLIB_platform_errno.h"

#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

/* On-disk layout returned by the Linux getdents64 syscall; dirent.c
   decodes this into struct dirent. Not referenced by any public
   function signature, so it does not leak into the ABI. */
struct linux_dirent64 {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[];
};


#ifndef REGTEST


/* Internal wrapper over the getdents64 syscall. Not exported. */
static ssize_t getdents64(int fd, void *dirp, size_t count)
{
    long rc = syscall(BPF_CALL_GETDENTS64, (long)fd, (long)dirp, (long)count);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (ssize_t)rc;
}

DIR *opendir(const char *pathname)
{
    DIR *dirp = NULL;
    int fd = open(pathname, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if(fd < 0) {
        return NULL;
    }
    dirp = fdopendir(fd);
    if(dirp == NULL) {
        int saved_errno = errno;
        close(fd);
        errno = saved_errno;
        return NULL;
    }
    return dirp;
}

DIR *fdopendir(int fd)
{
    DIR *dirp = (DIR *)malloc(sizeof(*dirp));
    if(dirp == NULL) {
        errno = ENOMEM;
        return NULL;
    }
    dirp->fd = fd;
    dirp->pos = 0;
    dirp->avail = 0;
    return dirp;
}

struct dirent *readdir(DIR *dirp)
{
    struct linux_dirent64 *ld;
    size_t name_len;

    if(dirp == NULL) {
        errno = EBADF;
        return NULL;
    }

    /* Refill the buffer once the previous chunk is exhausted. */
    if(dirp->pos >= dirp->avail) {
        ssize_t rc = getdents64(dirp->fd, dirp->buf, sizeof(dirp->buf));
        if(rc < 0) {
            return NULL;    /* errno already set by getdents64 */
        }
        if(rc == 0) {
            errno = 0;      /* end of directory */
            return NULL;
        }
        dirp->pos = 0;
        dirp->avail = (size_t)rc;
    }

    ld = (struct linux_dirent64 *)(void*)(dirp->buf + dirp->pos);

    memset(&dirp->entry, 0, sizeof(dirp->entry));
    dirp->entry.d_ino = ld->d_ino;
    dirp->entry.d_off = ld->d_off;
    dirp->entry.d_reclen = ld->d_reclen;
    dirp->entry.d_type = ld->d_type;
    /* ld->d_name is NUL-terminated by the kernel; cap the copy at
       sizeof(d_name) - 1 so the fixed buffer can never overflow. */
    name_len = strlen(ld->d_name);
    if(name_len >= sizeof(dirp->entry.d_name)) {
        name_len = sizeof(dirp->entry.d_name) - 1;
    }
    memcpy(dirp->entry.d_name, ld->d_name, name_len);

    dirp->pos += ld->d_reclen;
    return &dirp->entry;
}

int closedir(DIR *dirp)
{
    int rc;
    int saved_errno;

    if(dirp == NULL) {
        errno = EBADF;
        return -1;
    }
    rc = close(dirp->fd);
    saved_errno = errno;
    free(dirp);
    errno = saved_errno;
    return rc;
}

#endif
