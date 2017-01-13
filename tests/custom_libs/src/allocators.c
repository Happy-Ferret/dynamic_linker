#include "string.h"
#include "types.h"

#include "allocators_internal.h"

void *calloc(size_t nmemb, size_t size)
{
  void *ptr = malloc(nmemb * size);
  if (ptr)
    memset(ptr, 0, nmemb * size);
  return ptr;
}

void *realloc(void *ptr, size_t size)
{
  void *str = malloc(size);
  s_block *b = (s_block*)ptr;
  b--;
  size_t to_move = size < b->capacity ? size : b->capacity;
  memcpy((void*)str, (void*)ptr, to_move);
  return str;
}

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
