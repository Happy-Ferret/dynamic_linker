#include "allocators_internal.h"

void *memory = 0;

struct s_memory *get_memory_data()
{
  if (memory)
    return (s_memory*)memory;
  memory = (s_memory*)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  struct s_memory *mem = (s_memory*)memory;

  s_block *block = (s_block*)mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE,
      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  init_block(block, PAGE_SIZE - META_SIZE);
  mem->blocks = block;
  mem->last = block;
  mem->free = block;
  return mem;
}
