/* dirent.h - directory stream operations

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#ifndef DIRENT_H
#define DIRENT_H

#include <sys/types.h>
#include <stdint.h>

/* Directory entry exposed to user code (subset of POSIX/Linux). */
struct dirent {
    ino_t d_ino;
    off_t d_off;
    unsigned short d_reclen;
    unsigned char d_type;
    char d_name[256];
};

/* d_type values (match Linux <dirent.h>). */
enum {
    DT_UNKNOWN = 0,
    DT_FIFO = 1,
    DT_CHR = 2,
    DT_DIR = 4,
    DT_BLK = 6,
    DT_REG = 8,
    DT_LNK = 10,
    DT_SOCK = 12,
};

/* Size of the internal getdents64 read buffer. */
#define _DIRBUF_SIZE 4096

/* A directory stream. The buffer is filled in chunks via getdents64 and
   drained one entry at a time by readdir(). */
typedef struct {
    int fd;
    size_t pos;     /* offset of next entry to decode within buf */
    size_t avail;   /* valid bytes currently held in buf */
    struct dirent entry;
    char buf[_DIRBUF_SIZE];
} DIR;

#ifndef BPF_NO_SYSCALL
DIR *opendir(const char *pathname);
DIR *fdopendir(int fd);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);
#endif

#endif /* DIRENT_H */
