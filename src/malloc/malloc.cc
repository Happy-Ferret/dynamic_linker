#include "libmalloc.hh"
#include "types.hh"

extern "C"
{
  void *malloc(size_t size)
  {
    if (size == 0)
      return NULL;
    size = align(size + 10, sizeof (size_t));
    size = size < MIN_BLOCK_SIZE ? MIN_BLOCK_SIZE : size;
    void *ptr = get_free_block(size);

    if (!ptr)
      ptr = create_new_block(size);
    if (ptr)
      ptr = cook_free_block((s_block*)ptr, size);

    return ptr;
  }
}
