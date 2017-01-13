#include "defines.hh"
#include "syscall.hh"

int write(int fd, const char* str, size_t length)
{
#if __DEBUG
  int output = 0;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(1), "D"(fd), "S"(str), "d"(length)
      : "%r10", "%r8", "%r9"
  );
  return output;
#else
  (void)fd;
  (void)str;
  (void)length;
  return 0;
#endif
}

int open(const char *filename, int flags)
{
  int output = 0;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(2), "D"(filename), "S"(flags), "d"(0)
      : "%r10", "%r8", "%r9"
  );
  return output;
}

int close(int fd)
{
  int output = 0;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(3), "D"(fd)
      : "%rsi", "%rdx", "%r10", "%r8", "%r9"
  );
  return output;
}

int read(int fd, const char* buffer, size_t length)
{
  int output = 0;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(0), "D"(fd), "S"(buffer), "d"(length)
      : "%r10", "%r8", "%r9"
  );
  return output;
}

off_t lseek(int fd, off_t offset, int whence)
{
  int output = 0;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(8), "D"(fd), "S"(offset), "d"(whence)
      : "%r10", "%r8", "%r9"
  );
  return output;
}

int exit(int ecode)
{
  int output = 0;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(60), "D"(ecode)
      : "%rsi", "%rdx", "%r10", "%r8", "%r9"
  );
  return output;
}

void *mmap(void *addr, size_t len, int prot, int flags,
           int fildes, off_t off)
{
  register long r10 asm("r10") = flags;
  register long r8 asm("r8") = fildes;
  register long r9 asm("r9") = off;
  void *output = nullptr;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(9), "D"(addr), "S"(len), "d"(prot), "r"(r10), "r"(r8), "r"(r9)
  );
  return output;
}

void *mremap(void *old_addr, size_t old_size, size_t new_size, int flags)
{
  if (!old_addr)
    return 0;
  void *output = nullptr;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(9), "D"(old_addr), "S"(old_size), "d"(new_size), "c"(flags)
      : "%r10", "%r8", "%r9"
  );
  return output;
}

int munmap(void *addr, size_t len)
{
  if (!addr)
    return 0;
  int output = 0;
  asm volatile
  (
      "syscall;"
      : "=a"(output)
      : "a"(11), "D"(addr), "S"(len)
      : "%r10", "%r8", "%r9"
  );
  return output;
}
