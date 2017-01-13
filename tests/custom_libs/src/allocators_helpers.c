#include "allocators_internal.h"

size_t align(size_t value, size_t divisor)
{
  return (~(divisor - 1) & (value - 1)) + divisor;
}

void init_block(struct s_block *b, size_t size)
{
  if (!b)
    return;
  b->capacity = size;
  b->next_mem = NULL;
  b->prev_sto = NULL;
  b->next_sto = NULL;
  b->magic_a = 42;
  b->magic_b = 42;
}

int is_adjacent(struct s_block *block, struct s_block *next)
{
  struct s_block *cp_next =
                          (s_block*)forward_ptr_c(block + 1, block->capacity);
  return cp_next == next;
}

void *forward_ptr_c(void *ptr, size_t offset)
{
  char *c = (char*)ptr;
  return c + offset;
}
