#include "syscall.h"

void print_some2(const char *str, unsigned long len)
{
  write(1, str, len);
}
