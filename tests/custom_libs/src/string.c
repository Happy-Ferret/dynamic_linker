#include "types.h"
#include "defines.h"
#include "allocators.h"
#include "syscall.h"

size_t strlen(const char* str)
{
  size_t len = 0;
  for (; *str; str++)
    len++;
  return len;
}

int strcmp(const char* a, const char* b)
{
  for (; *a && *b; a++, b++)
    if (*a != *b)
      break;
  return *a - *b;
}

int strncmp(const char* a, const char* b, size_t len)
{
  for (; *a && *b && len > 1; a++, b++, len--)
    if (*a != *b)
      break;
  return *a - *b;
}

void *memset(void *s, int c, size_t size)
{
  char *start = (char*)s;
  for (size_t i = 0; i < size; i++)
    start[i] = (char)c;
  return s;
}

void *memcpy(void *dest, void *src, size_t n)
{
  char *d = (char*)dest;
  char *s = (char*)src;
  for (size_t i = 0; i < n; i++)
    *d++ = *s++;
  return dest;
}

size_t strpos(const char *str, char c)
{
  size_t pos = 0;
  for (; str[pos]; pos++)
    if (str[pos] == c)
      break;
  return pos;
}

char* substr(const char *str, size_t start, size_t len)
{
  char *out = (char*)calloc(1, len + 1);
  if (start > strlen(str))
    return out;
  for (size_t i = 0; i < len && str[start + i]; i++)
    out[i] = str[start + i];
  return out;
}

char *strdup(const char *in)
{
  char *out = (char*)calloc(strlen(in), 1);
  for (size_t i = 0; in[i]; i++)
    out[i] = in[i];
  return out;
}

char *strcat(const char *a, const char *b)
{
  char *c = (char*)malloc(strlen(a) + strlen(b) + 1);
  size_t pos = strlen(a);
  for (size_t i = 0; i < strlen(a); i++)
    c[i] = a[i];
  for (size_t i = 0; i < strlen(b); i++)
    c[pos + i] = b[i];
  c[pos + strlen(b)] = 0;
  return c;
}

char *strchr(const char *str, int c)
{
  for (; *str; str++)
    if (*str == c)
      return (void*)str;
  return NULL;
}
