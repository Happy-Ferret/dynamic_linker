#include "libmalloc.hh"

extern "C"
{
  void free(void *ptr)
  {
    if (!ptr)
      return;
    return;
    struct s_block *block = (s_block*)ptr;
    block--;
    store_free_block(block);
    merge_block(block);
  }
}
