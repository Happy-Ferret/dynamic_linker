#include "libmalloc.hh"
#include "strings.hh"

extern "C"
{
  void *realloc(void *ptr, size_t size)
  {
    /*
    if (size == 0)
    {
      free(ptr);
      return nullptr;
    }
    if (!ptr)
      return malloc(size);
    struct s_block *block = (s_block*)ptr;
    block--;
    size = align(size, sizeof (size_t));
    size = size < MIN_BLOCK_SIZE ? MIN_BLOCK_SIZE : size;
    size_t to_move = size < block->capacity ? size : block->capacity;

    if (size < block->capacity)
      return ptr;
    void *new_block = malloc(size);
    if (new_block)
      memcpy(new_block, ptr, to_move);

    free(ptr);
    return new_block ? new_block : ptr;
    */
    void *str = malloc(size);
    s_block *b = (s_block*)ptr;
    b--;
    size_t to_move = size < b->capacity ? size : b->capacity;
    memcpy((void*)str, (void*)ptr, to_move);
    return str;
  }
}
