#include "syscall.h"
#include "types.h"

void print_internal(const char *str, size_t len, int fd)
{
  write(fd, str, len);
}

void print_some3(const char *str, size_t len)
{
  print_internal(str, len, 1);
}
