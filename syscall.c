
/* $Id$ */

#include <stdlib.h>
#include <reent.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <libopencm3/stm32/usart.h>
#include <errno.h>

#include <syscall.h>
#include <buffer.h>
#include <uastdio.h>

#undef errno
extern int errno;

char *__env[1] = { 0 };

char **environ = __env;

int _execve(char *name, char **argv, char **env) {
    errno = ENOMEM;
    return -1;
}

int _fork(void) {
    errno = EAGAIN;
    return -1;
}

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    errno = EINVAL;
    return -1;
}

int _exit() {
    while(1);
}

int _isatty(int file) {
    return 1;
}

int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _link(char *old, char *new) {
    errno = EMLINK;
    return -1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _open(const char *name, int flags, int mode) {
    return -1;
}

#define STDIN   0
#define STDOUT  1
#define STDERR  3


int _read(int file, char *ptr, int len) {
    uint8_t data = 0;
    int i = 0;
    while (i < len) {
        ptr[i++] = 'z';
    }
    return i;
}


int _write(int file, char *ptr, int len) {
    int i;
    if (file == STDOUT | file == STDERR) {
        for (i = 0; i < len; i++) {
            buffer_put_byte(&stdout_buffer, *ptr++);
        }
        return len;
    }
    return 0;
}

int _stat(char *file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

int _close(int file) {
    return -1;
}


int _times(struct tms *buf) {
    return -1;
}

int _unlink(char *name) {
    errno = ENOENT;
    return -1;
}

int _wait(int *status) {
    errno = ECHILD;
    return -1;
}

void * _sbrk(int incr) {

    extern void *__heap_start;

    void * prev_heap_ptr;
    static void * heap_ptr;

    if (heap_ptr == 0) {
        heap_ptr = (void *)&__heap_start;
    }

    prev_heap_ptr = heap_ptr;

    void * stack_ptr = (void *) __get_SP();
    if (heap_ptr + incr > stack_ptr) {
        errno = ENOMEM;
        _write(1, "Heap and stack collision\r\n", 26);
        abort();
    }

    heap_ptr += incr;

    return (void *) prev_heap_ptr;
}

/* EOF */
