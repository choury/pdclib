/* BPF syscall wrappers.
   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>

#include "bpf_call.h"
#include "pdclib/_PDCLIB_config.h"
#include "pdclib/_PDCLIB_platform_errno.h"

#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <sys/time.h>
#include <sys/times.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <utime.h>

#ifndef REGTEST

__attribute__((noreturn)) void _exit(int status)
{
    syscall(BPF_CALL_EXIT, (long)status);
    __builtin_unreachable();
}

int execve(const char* pathname, char* const argv[], char* const envp[])
{
    long rc = syscall(BPF_CALL_EXECVE, (long)pathname, (long)argv, (long)envp);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int dup2(int oldfd, int newfd)
{
    /* dup3 在 oldfd==newfd 时返回 EINVAL，但 POSIX 要求 dup2(a,a) 在 a 有效时返回 a。 */
    if(oldfd == newfd) {
        return oldfd;
    }
    long rc = syscall(BPF_CALL_DUP3, (long)oldfd, (long)newfd, (long)0);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int pipe2(int pipefd[2], int flags)
{
    long rc = syscall(BPF_CALL_PIPE2, (long)pipefd, (long)flags);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int pipe(int pipefd[2])
{
    return pipe2(pipefd, 0);
}

int close(int fd)
{
    long rc = syscall(BPF_CALL_CLOSE, (long)fd);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

ssize_t read(int fd, void *buf, size_t count)
{
    long rc = syscall(BPF_CALL_READ, (long)fd, (long)buf, (long)count);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (ssize_t)rc;
}

ssize_t write(int fd, const void *buf, size_t count)
{
    long rc = syscall(BPF_CALL_WRITE, (long)fd, (long)buf, (long)count);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (ssize_t)rc;
}

int unlink(const char *pathname)
{
    return unlinkat(AT_FDCWD, pathname, 0);
}

int unlinkat(int dirfd, const char *pathname, int flags)
{
    long rc = syscall(BPF_CALL_UNLINKAT, (long)dirfd, (long)pathname, (long)flags);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int mkdir(const char *pathname, mode_t mode)
{
    long rc = syscall(BPF_CALL_MKDIRAT, (long)AT_FDCWD, (long)pathname, (long)mode);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int mknodat(int dirfd, const char *pathname, mode_t mode, dev_t dev)
{
    (void)dirfd;
    (void)pathname;
    (void)mode;
    (void)dev;
    errno = EPERM;
    return -1;
}

int mknod(const char *pathname, mode_t mode, dev_t dev)
{
    return mknodat(AT_FDCWD, pathname, mode, dev);
}

int symlink(const char *target, const char *linkpath)
{
    return symlinkat(target, AT_FDCWD, linkpath);
}

int symlinkat(const char *target, int newdirfd, const char *linkpath)
{
    long rc = syscall(BPF_CALL_SYMLINKAT, (long)target, (long)newdirfd, (long)linkpath);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int linkat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath, int flags)
{
    long rc = syscall(BPF_CALL_LINKAT, (long)olddirfd, (long)oldpath, (long)newdirfd, (long)newpath, (long)flags);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int link(const char *oldpath, const char *newpath)
{
    return linkat(AT_FDCWD, oldpath, AT_FDCWD, newpath, 0);
}

int chmod(const char *path, mode_t mode)
{
    return fchmodat(AT_FDCWD, path, mode, 0);
}

int fchmod(int fd, mode_t mode)
{
    long rc = syscall(BPF_CALL_FCHMODAT, (long)fd, (long)"", (long)mode, (long)AT_EMPTY_PATH);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int fchmodat(int dirfd, const char *pathname, mode_t mode, int flags)
{
    long rc = syscall(BPF_CALL_FCHMODAT, (long)dirfd, (long)pathname, (long)mode, (long)flags);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int rmdir(const char *pathname)
{
    /* AT_REMOVEDIR=0x200（删目录）；BPF_SYS_RMDIR 已废弃，统一走 unlinkat。 */
    long rc = syscall(BPF_CALL_UNLINKAT, (long)AT_FDCWD, (long)pathname, (long)0x200);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int renameat(int olddirfd, const char *oldpath, int newdirfd, const char *newpath)
{
    long rc = syscall(BPF_CALL_RENAMEAT2, (long)olddirfd, (long)oldpath, (long)newdirfd, (long)newpath, (long)0);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int readlink(const char *pathname, char *buf, size_t bufsiz)
{
    long rc = syscall(BPF_CALL_READLINKAT, (long)AT_FDCWD, (long)pathname, (long)buf, (long)bufsiz);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

pid_t getpid(void)
{
    long rc = syscall(BPF_CALL_GETPID);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

pid_t getppid(void)
{
    long rc = syscall(BPF_CALL_GETPPID);
    if(rc < 0) {
        errno = (int)-rc;
        return (pid_t)-1;
    }
    return (pid_t)rc;
}

int fork(void)
{
    long rc = syscall(BPF_CALL_FORK);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int vfork(void)
{
    return fork();
}

int waitpid(int pid, int *status, int options)
{
    long rc = syscall(BPF_CALL_WAITPID, (long)pid, (long)status, (long)options);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int fchdir(int fd)
{
    long rc = syscall(BPF_CALL_FCHDIR, (long)fd);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int chdir(const char *path)
{
    int fd, rc, saved_errno;
    fd = open(path, O_RDONLY | O_DIRECTORY | O_CLOEXEC);
    if(fd < 0) {
        return -1;
    }
    rc = fchdir(fd);
    saved_errno = errno;
    close(fd);
    errno = saved_errno;
    return rc;
}

char *getcwd(char *buf, size_t size)
{
    long rc = syscall(BPF_CALL_GETCWD, (long)buf, (long)size);
    if(rc < 0) {
        errno = (int)-rc;
        return NULL;
    }
    return buf;
}


uid_t getuid(void)
{
    return (uid_t)0;
}

int chown(const char *path, uid_t owner, gid_t group)
{
    (void)path;
    (void)owner;
    (void)group;
    errno = EPERM;
    return -1;
}

int fchown(int fd, uid_t owner, gid_t group)
{
    (void)fd;
    (void)owner;
    (void)group;
    errno = EPERM;
    return -1;
}

int lchown(const char *pathname, uid_t owner, gid_t group)
{
    (void)pathname;
    (void)owner;
    (void)group;
    errno = EPERM;
    return -1;
}

int fchownat(int dirfd, const char *pathname, uid_t owner, gid_t group, int flags)
{
    (void)dirfd;
    (void)pathname;
    (void)owner;
    (void)group;
    (void)flags;
    errno = EPERM;
    return -1;
}

uid_t geteuid(void)
{
    return getuid();
}

gid_t getgid(void)
{
    return (gid_t)0;
}

gid_t getegid(void)
{
    return getgid();
}

int getgroups(int size, gid_t list[])
{
    if(size < 0) {
        errno = EINVAL;
        return -1;
    }
    if(size > 0 && list == NULL) {
        errno = EFAULT;
        return -1;
    }
    return 0;
}

int dup(int fd)
{
    long rc = syscall(BPF_CALL_DUP, (long)fd);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int open3(const char *pathname, int flags, mode_t mode)
{
    long rc = syscall(BPF_CALL_OPENAT, (long)AT_FDCWD, (long)pathname, (long)flags, (long)mode);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int creat(const char *pathname, mode_t mode)
{
    return open3(pathname, O_WRONLY | O_CREAT | O_TRUNC, mode);
}

int openat4(int dirfd, const char *pathname, int flags, mode_t mode)
{
    long rc = syscall(BPF_CALL_OPENAT, (long)dirfd, (long)pathname, (long)flags, (long)mode);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

off64_t lseek(int fd, off64_t offset, int whence)
{
    long rc = syscall(BPF_CALL_LSEEK, (long)fd, (long)offset, (long)whence);
    if(rc < 0) {
        errno = (int)-rc;
        return (off64_t)-1;
    }
    return (off64_t)rc;
}

int truncate(const char *pathname, off_t length)
{
    long rc = syscall(BPF_CALL_TRUNCATE, (long)pathname, (long)length);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int ftruncate(int fd, off_t length)
{
    long rc = syscall(BPF_CALL_FTRUNCATE, (long)fd, (long)length);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int fcntl3(int fd, int cmd, long arg)
{
    long rc = syscall(BPF_CALL_FCNTL, (long)fd, (long)cmd, (long)arg);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

void* mmap(void *addr, size_t length, int prot, int flags, int fd, uint64_t offset)
{
    long rc = syscall(BPF_CALL_MMAP, (long)addr, (long)length, (long)prot, (long)flags, (long)fd, (long)offset);
    if(rc < 0) {
        errno = (int)-rc;
        return MAP_FAILED;
    }
    return (void *)rc;
}

int munmap(void* addr, size_t length)
{
    long rc = syscall(BPF_CALL_MUNMAP, (long)addr, (long)length);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
    long rc = syscall(BPF_CALL_CLOCK_GETTIME, (long)clock_id, (long)tp);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int gettimeofday(struct timeval* tv, struct timezone* tz)
{
    if (tv) {
        struct timespec ts;
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
            return -1;
        }
        tv->tv_sec = ts.tv_sec;
        tv->tv_usec = ts.tv_nsec / 1000;
    }
    if (tz) {
        // timezone is obsolete and not supported by clock_gettime
        tz->tz_minuteswest = 0;
        tz->tz_dsttime = 0;
    }
    return 0;
}

int nanosleep(const struct timespec* req, struct timespec* rem)
{
    long rc = syscall(BPF_CALL_NANOSLEEP, (long)req, (long)rem);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return 0;
}

clock_t times(struct tms *buf)
{
    const long ticks_per_sec = 100;
    struct timespec ts;
    struct timespec ts_mono;
    if (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts) == -1) {
        return (clock_t)-1;
    }
    if (buf) {
        buf->tms_utime = ts.tv_sec * ticks_per_sec + ts.tv_nsec * ticks_per_sec / 1000000000;
        buf->tms_stime = 0;
        buf->tms_cutime = 0;
        buf->tms_cstime = 0;
    }
    if (clock_gettime(CLOCK_MONOTONIC, &ts_mono) == -1) {
        return (clock_t)-1;
    }
    return (clock_t)(ts_mono.tv_sec * ticks_per_sec + ts_mono.tv_nsec * ticks_per_sec / 1000000000);
}

int ioctl(int fd, unsigned long request, void *arg)
{
    long rc = syscall(BPF_CALL_IOCTL, (long)fd, (long)request, (long)arg);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int getrlimit(int resource, struct rlimit *rlim)
{
    if (rlim) {
        rlim->rlim_cur = RLIM_INFINITY;
        rlim->rlim_max = RLIM_INFINITY;
    }
    return 0;
}

int setrlimit(int resource, const struct rlimit *rlim)
{
    return 0;
}

int getpriority(int which, int who)
{
    (void)which;
    (void)who;
    return 0;
}

int setpriority(int which, int who, int prio)
{
    (void)which;
    (void)who;
    (void)prio;
    return 0;
}

int stat(const char *path, struct stat *buf)
{
    return fstatat(AT_FDCWD, path, buf, 0);
}

int lstat(const char *path, struct stat *buf)
{
    return fstatat(AT_FDCWD, path, buf, AT_SYMLINK_NOFOLLOW);
}

int fstat(int fd, struct stat *buf)
{
    return fstatat(fd, "", buf, AT_EMPTY_PATH);
}

int fstatat(int dirfd, const char *pathname, struct stat *buf, int flags)
{
    /* VM 只提供 do_statx（5 参），无 fstatat handler。发 statx 拿 struct statx，
     * 再逐字段转成 struct stat（与 musl fstatat_statx 同构）。mask 取
     * STATX_BASIC_STATS 覆盖 struct stat 全部字段。 */
    struct statx stx;
    long rc = syscall(BPF_CALL_STATX, (long)dirfd, (long)pathname,
                      (long)(flags | AT_NO_AUTOMOUNT), (long)STATX_BASIC_STATS,
                      (long)&stx);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    *buf = (struct stat){0};
    buf->st_dev = makedev(stx.stx_dev_major, stx.stx_dev_minor);
    buf->st_ino = stx.stx_ino;
    buf->st_mode = stx.stx_mode;
    buf->st_nlink = stx.stx_nlink;
    buf->st_uid = stx.stx_uid;
    buf->st_gid = stx.stx_gid;
    buf->st_rdev = makedev(stx.stx_rdev_major, stx.stx_rdev_minor);
    buf->st_size = stx.stx_size;
    buf->st_blksize = stx.stx_blksize;
    buf->st_blocks = stx.stx_blocks;
    buf->st_atim.tv_sec = stx.stx_atime.tv_sec;
    buf->st_atim.tv_nsec = stx.stx_atime.tv_nsec;
    buf->st_mtim.tv_sec = stx.stx_mtime.tv_sec;
    buf->st_mtim.tv_nsec = stx.stx_mtime.tv_nsec;
    buf->st_ctim.tv_sec = stx.stx_ctime.tv_sec;
    buf->st_ctim.tv_nsec = stx.stx_ctime.tv_nsec;
    return 0;
}

mode_t umask(mode_t mask)
{
    long rc = syscall(BPF_CALL_UMASK, (long)mask);
    if(rc < 0) {
        errno = (int)-rc;
        return (mode_t)-1;
    }
    return (mode_t)rc;
}

int sigaction(int signo, const struct sigaction *act, struct sigaction *oldact)
{
    long rc = syscall(BPF_CALL_SIGACTION, (long)signo, (long)act, (long)oldact);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int kill(pid_t pid, int sig)
{
    long rc = syscall(BPF_CALL_KILL, (long)pid, (long)sig);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int utimensat(int dirfd, const char *pathname, const struct timespec times[2], int flags)
{
    long rc = syscall(BPF_CALL_UTIMENSAT, (long)dirfd, (long)pathname, (long)times, (long)flags);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int futimens(int fd, const struct timespec times[2])
{
    return utimensat(fd, "", times, AT_EMPTY_PATH);
}

int utime(const char *filename, const struct utimbuf *times)
{
    struct timespec ts[2];
    if (times) {
        ts[0].tv_sec = times->actime;
        ts[0].tv_nsec = 0;
        ts[1].tv_sec = times->modtime;
        ts[1].tv_nsec = 0;
        return utimensat(AT_FDCWD, filename, ts, 0);
    } else {
        return utimensat(AT_FDCWD, filename, NULL, 0);
    }
}

int utimes(const char *filename, const struct timeval times[2])
{
    struct timespec ts[2];
    if (times) {
        ts[0].tv_sec = times[0].tv_sec;
        ts[0].tv_nsec = times[0].tv_usec * 1000;
        ts[1].tv_sec = times[1].tv_sec;
        ts[1].tv_nsec = times[1].tv_usec * 1000;
        return utimensat(AT_FDCWD, filename, ts, 0);
    } else {
        return utimensat(AT_FDCWD, filename, NULL, 0);
    }
}

int lutimes(const char *filename, const struct timeval times[2])
{
    struct timespec ts[2];
    if (times) {
        ts[0].tv_sec = times[0].tv_sec;
        ts[0].tv_nsec = times[0].tv_usec * 1000;
        ts[1].tv_sec = times[1].tv_sec;
        ts[1].tv_nsec = times[1].tv_usec * 1000;
        return utimensat(AT_FDCWD, filename, ts, AT_SYMLINK_NOFOLLOW);
    } else {
        return utimensat(AT_FDCWD, filename, NULL, AT_SYMLINK_NOFOLLOW);
    }
}

int futimes(int fd, const struct timeval times[2])
{
    struct timespec ts[2];
    if (times) {
        ts[0].tv_sec = times[0].tv_sec;
        ts[0].tv_nsec = times[0].tv_usec * 1000;
        ts[1].tv_sec = times[1].tv_sec;
        ts[1].tv_nsec = times[1].tv_usec * 1000;
        return utimensat(fd, "", ts, AT_EMPTY_PATH);
    }
    return utimensat(fd, "", NULL, AT_EMPTY_PATH);
}

int faccessat(int dirfd, const char *pathname, int mode, int flags)
{
    long rc = syscall(BPF_CALL_FACCESSAT, (long)dirfd, (long)pathname, (long)mode, (long)flags);
    if(rc < 0) {
        errno = (int)-rc;
        return -1;
    }
    return (int)rc;
}

int access(const char *pathname, int mode)
{
    return faccessat(AT_FDCWD, pathname, mode, 0);
}

pid_t wait(int *status)
{
    return waitpid(-1, status, 0);
}

int chroot(const char *path)
{
    /* chroot is not supported in BPF VM */
    (void)path;
    errno = EPERM;
    return -1;
}

int gethostname(char *name, size_t len)
{
    const char *hostname = "bpfvm";
    size_t hlen = strlen(hostname);
    if (len < hlen + 1) {
        errno = ENAMETOOLONG;
        return -1;
    }
    memcpy(name, hostname, hlen + 1);
    return 0;
}

int sethostname(const char *name, size_t len)
{
    (void)name;
    (void)len;
    errno = EPERM;
    return -1;
}

char *getlogin(void)
{
    static char *login = NULL;
    if (!login)
        login = getenv("LOGNAME");
    if (!login)
        login = getenv("USER");
    return login;
}

pid_t setsid(void)
{
    /* stub: pretend success, return current pid */
    return getpid();
}

pid_t getpgrp(void)
{
    /* stub: return current pid */
    return getpid();
}

int mkfifoat(int dirfd, const char *pathname, mode_t mode)
{
    return mknodat(dirfd, pathname, mode | S_IFIFO, 0);
}

int mkfifo(const char *pathname, mode_t mode)
{
    return mkfifoat(AT_FDCWD, pathname, mode);
}

#endif
