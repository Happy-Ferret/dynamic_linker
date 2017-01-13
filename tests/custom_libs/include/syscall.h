#pragma once

#include "types.h"

size_t read(int fd, void *buf, size_t count);
size_t write(int fd, const char* str, size_t count);
int open(const char* path, int flag);
void close(int fd);
off_t lseek(int fildes, off_t offset, int whence);
void *mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
int munmap(void *addr, size_t len);
int brk(void *addr);
void *mremap(void *old_addr, size_t old_size, size_t new_size, int flag);
int pipe(int fd[2]);
int dup(int fd);
int dup2(int fd1, int fd2);
int fork(void);
void exit(int ecode);
