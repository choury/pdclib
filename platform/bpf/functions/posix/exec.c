/* exec family functions

   This file is part of the Public Domain C Library (PDCLib).
   Permission is granted to use, modify, and / or redistribute at will.
*/

#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

extern char **environ;

int execv(const char *path, char *const argv[])
{
    return execve(path, argv, environ);
}

int execvp(const char *file, char *const argv[])
{
    const char *path_env;
    size_t file_len;
    char buf[PATH_MAX];
    const char *p;
    bool saw_eacces = false;

    if (strchr(file, '/')) {
        return execve(file, argv, environ);
    }
    
    path_env = getenv("PATH");
    if (!path_env) {
        path_env = "/bin:/usr/bin";
    }
    
    file_len = strlen(file);
    p = path_env;
    
    while (*p) {
        const char *end = strchr(p, ':');
        size_t len = end ? (size_t)(end - p) : strlen(p);
        
        if (len + 1 + file_len + 1 > sizeof(buf)) {
             if (!end) break;
             p = end + 1;
             continue;
        }
        
        if (len > 0) {
            memcpy(buf, p, len);
            buf[len] = '/';
            memcpy(buf + len + 1, file, file_len);
            buf[len + 1 + file_len] = '\0';
        } else {
            memcpy(buf, file, file_len);
            buf[file_len] = '\0';
        }
        
        execve(buf, argv, environ);
        if (errno == EACCES) {
            saw_eacces = true;
        } else if (errno != ENOENT) {
            return -1;
        }
        
        if (!end) break;
        p = end + 1;
    }
    
    if (saw_eacces) {
        errno = EACCES;
    } else {
        errno = ENOENT;
    }
    return -1;
}

int execl(const char *path, const char *arg, ...)
{
    va_list ap;
    int argc = 0;
    char **argv;
    int ret;
    const char *a;

    /* 先数一遍参数个数（直到 NULL 终止符） */
    va_start(ap, arg);
    for (a = arg; a != NULL; a = va_arg(ap, const char *)) {
        argc++;
    }
    va_end(ap);

    argv = malloc((argc + 1) * sizeof(char *));
    if (!argv) {
        errno = ENOMEM;
        return -1;
    }

    /* 再读一遍，填充 argv */
    va_start(ap, arg);
    a = arg;
    for (int j = 0; j < argc; j++) {
        argv[j] = (char *)a;
        a = va_arg(ap, const char *);
    }
    argv[argc] = NULL;
    va_end(ap);

    ret = execve(path, argv, environ);
    free(argv);
    return ret;
}

int execle(const char *path, const char *arg, ...)
{
    va_list ap;
    int argc = 0;
    char **argv;
    char *const *envp = NULL;
    int ret;
    const char *a;

    /* 数参数个数，envp 是 NULL 之后的下一个参数 */
    va_start(ap, arg);
    for (a = arg; a != NULL; a = va_arg(ap, const char *)) {
        argc++;
    }
    envp = va_arg(ap, char *const *);
    va_end(ap);

    argv = malloc((argc + 1) * sizeof(char *));
    if (!argv) {
        errno = ENOMEM;
        return -1;
    }

    va_start(ap, arg);
    a = arg;
    for (int j = 0; j < argc; j++) {
        argv[j] = (char *)a;
        a = va_arg(ap, const char *);
    }
    argv[argc] = NULL;
    /* 跳过 NULL 终止符，取 envp */
    va_arg(ap, const char *);
    envp = va_arg(ap, char *const *);
    va_end(ap);

    ret = execve(path, argv, envp);
    free(argv);
    return ret;
}

int execlp(const char *file, const char *arg, ...)
{
    va_list ap;
    int argc = 0;
    char **argv;
    int ret;
    const char *a;

    va_start(ap, arg);
    for (a = arg; a != NULL; a = va_arg(ap, const char *)) {
        argc++;
    }
    va_end(ap);

    argv = malloc((argc + 1) * sizeof(char *));
    if (!argv) {
        errno = ENOMEM;
        return -1;
    }

    va_start(ap, arg);
    a = arg;
    for (int j = 0; j < argc; j++) {
        argv[j] = (char *)a;
        a = va_arg(ap, const char *);
    }
    argv[argc] = NULL;
    va_end(ap);

    ret = execvp(file, argv);
    free(argv);
    return ret;
}