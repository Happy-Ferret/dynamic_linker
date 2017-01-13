#include "defines.hh"
#include "libmalloc.hh"
#include "strings.hh"
#include "syscall.hh"
#include "types.hh"
#include "utils.hh"
//#include "variadic.hh"

void *forward_ptr(void *ptr, size_t offset)
{
  char *c = static_cast<char*>(ptr);
  return static_cast<void*>(c + offset);
}

void fputs(int fd, const char* text)
{
#ifdef __DEBUG
  if (fd == STDERR)
    write(fd, "\033[91m", strlen("\033[91m"));
  write(fd, text, strlen(text));
  write(fd, "\n", 1);
  if (fd == STDERR)
    write(fd, "\033[0m", strlen("\033[0m"));
#else
  (void)fd;
  (void)text;
#endif
}

void print(const char *str)
{
#ifdef __DEBUG
  write(STDERR, str, strlen(str));
#else
  (void)str;
#endif

}

void print(const char *a, const char *b)
{ print(a); print(b);}
void print(const char *a, const char *b, const char* c)
{ print(a); print(b); print(c);}
void print(const char *a, const char *b, const char* c, const char* d)
{ print(a); print(b); print(c); print(d);}
void print(const char *a, const char *b, const char* c,
           const char* d, const char* e)
{ print(a); print(b); print(c); print(d); print(e);}
void print(const char *a, const char *b, const char* c,
           const char* d, const char* e, const char *f)
{ print(a); print(b); print(c); print(d); print(e); print(f);}
void print(const char *a, const char *b, const char* c,
           const char* d, const char* e, const char *f, const char *g)
{ print(a, b, c, d, e, f); print (g); }


bool check_magic(const unsigned char m[EI_NIDENT])
{
  return m[0] == 0x7f && m[1] == 0x45 && m[2] == 0x4c && m[3] == 0x46
      && m[4] == 0x02 && m[5] == 0x01 && m[6] == 0x01
      && (m[7] == 0x00 || m[7] == 0x03);
}

extern "C"
{
  void *_Znwm(unsigned long size)
  {
    return malloc(size);
  }
  void _ZdlPvm(void *ptr, unsigned long size)
  {
    (void)size;
    free(ptr);
  }

  void _ZdlPv(void *ptr)
  {
    free(ptr);
  }
}

char* ztoa16(unsigned long n)
{
  if (n == 0)
    return strdup("0");
  size_t len = 0;
  for (unsigned long c = n; c > 0; c = c / 16)
    len++;

  char *out = (char*)calloc(1, len + 1);
  for (size_t pos = len - 1; n > 0; pos--)
  {
    size_t unit = n % 16;
    if (unit > 9)
      out[pos] = 'a' + unit - 10;
    else
      out[pos] = '0' + unit;
    n = n / 16;
  }
  return out;
}

char* ztoa(unsigned long n)
{
  if (n == 0)
    return strdup("0");
  size_t len = 0;
  for (unsigned long c = n; c > 0; c = c / 10)
    len++;

  char *out = (char*)calloc(1, len + 1);
  for (size_t pos = len - 1; n > 0; pos--)
  {
    size_t unit = n % 10;
    out[pos] = '0' + unit;
    n = n / 10;
  }
  return out;
}

char* getenv(char** env, const char *name)
{
  while (*env)
  {
    size_t pos = strpos(*env, '=');
    if (!strncmp(name, *env, pos))
      return substr(*env, pos + 1, strlen(*env) - pos);
    env++;
  }
  return nullptr;
}

size_t count_args(const char* format)
{
  size_t len = 0;
  for (; *format; format++)
  {
    if (*format == '%')
    {
      format++;
      if (*format == '%')
        len++;
    }
  }
  return len;
}

void printf(const char* format, ...)
{
#if __DEBUG
  VA_START();
  size_t args = 0;
  (void)args;
  for (; *format; format++)
  {
    if (*format == '%')
    {
      format++;
      if (*format == 's')
      {
        char *str = VA_NEXT(v, char*);
        if (str)
        {
          size_t len = strlen(str);
          write(1, str, len);
        }
      }
      else
        write(1, "%%", 2);
    }
    else
      write(1, format, 1);
  }
#else
  (void)format;
#endif
}
