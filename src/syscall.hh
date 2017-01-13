#pragma once

#include "types.hh"
#include "defines.hh"

int open(const char *filename, int flags);
int close(int fd);

int write(int fd, const char* str, size_t length);
int read(int fd, const char* buffer, size_t length);
off_t lseek(int fd, off_t offset, int whence);

int exit(int ecode);

void* mmap(void *addr, size_t len, int prot, int flags,
           int fildes, off_t off);
void* mremap(void *old_addr, size_t old_size, size_t new_size, int flags);
int munmap(void *addr, size_t len);
