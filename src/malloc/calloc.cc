#include "libmalloc.hh"
#include "strings.hh"

extern "C"
{
  void *calloc(size_t nmemb, size_t size)
  {
    void *ptr = malloc(nmemb * size);
    if (ptr)
      memset(ptr, 0, nmemb * size);
    return ptr;
  }
}
