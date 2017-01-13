#include "syscall.h"
#include "string.h"
#include "allocators.h"
#include "libdl.h"
#include "print.h"
#include "print2.h"
#include "print3.h"
#include "libc_my.h"

#define FIRST_LIB 1
#define SECOND_LIB 1
#define SELF 1
#define SIMPLE_REC 1
#define LOOP_REC 0
#define DYNAMIC_LOAD 1

unsigned long len(const char *str)
{
  unsigned long len = 0;
  for (; *str; str++)
    len++;
  return len;
}

int main(int argc, char** argv)
{
  (void)argc;
  (void)argv;
  for (int i = 1; i < argc; i++)
  {
    write(1, argv[i], len(argv[i]));
    write(1, "\n", 1);
  }

#if SECOND_LIB
  print_some(1, "second\n", 7);
#endif

#if SIMPLE_REC
  print_some2("simple_rec\n", 11);
#endif

#if LOOP_REC
  const char *str = "abcdef";
  if (strlen(str) != 6)
    return 42;
  strchr(str, 'c');
#endif

#if SELF
  print_some3("print_some_3\n", 13);
#endif

#if DYNAMIC_LOAD
  void *handle = my_dlopen("libprint.so", 0);
  if (handle)
  {
    size_t addr = (size_t)my_dlsym(handle, "print_some");
    void(*fptr)(int fd, const char* str, size_t len) = (void*)addr;
    print_some(1, "Will jump\n", 10);
    fptr(1, "JMP\n", 4);
    my_dlclose(handle);
  }

  void *handle2 = my_dlopen("../tests/libprint.so", 0);
  if (handle2)
    my_dlclose(handle2);

  void *handle3 = my_dlopen("../tests/libunknown.so", 0);
  if (handle3)
    print_some(1, "FAIL\n", 5);
  my_dlerror();
#endif

  /* Simple link with libsyscall (no deps) */
#if FIRST_LIB
  dup(1);
  fork();
  write(1, "OK\n", 3);
  close(0);
  close(1);
  close(2);
#endif

#if FIRST_LIB
  exit(0);
#endif
  return 0;
}
