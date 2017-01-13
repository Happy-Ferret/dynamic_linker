#include "allocators_internal.h"

struct s_block *split_block(struct s_block *block, size_t size)
{
  if (block->capacity <= size + META_SIZE + MIN_BLOCK_SIZE)
    return block;
  struct s_block *new_block = (s_block*)forward_ptr_c(block + 1, size);
  init_block(new_block, block->capacity - size - META_SIZE);
  block->capacity = size;
  new_block->next_mem = block->next_mem;
  new_block->next_sto = block->next_sto;
  new_block->prev_sto = block;
  if (new_block->next_sto)
    new_block->next_sto->prev_sto = new_block;
  block->next_mem = new_block;
  block->next_sto = new_block;

  s_memory *memory = get_memory_data();
  if (memory->last == block)
    memory->last = new_block;
  return block;
}

void merge_block(struct s_block *block)
{
  while (block->prev_sto && is_adjacent(block->prev_sto, block))
    block = block->prev_sto;
  size_t capacity = block->capacity;
  struct s_block *start = block;
  while (block->next_sto && is_adjacent(block, block->next_sto))
  {
    capacity += META_SIZE + block->next_sto->capacity;
    block = block->next_sto;
  }
  if (start == block)
    return;
  start->capacity = capacity;
  start->next_sto = block->next_sto;
  start->next_mem = block->next_mem;
  if (block->next_sto)
    block->next_sto->prev_sto = start;
  if (!block->next_mem)
  {
    struct s_memory *mem = get_memory_data();
    mem->last = start;
  }
}

struct s_block *create_new_block(size_t size)
{
  size = align(size + META_SIZE, PAGE_SIZE);
  struct s_memory *mem = get_memory_data();
  struct s_block *block = (s_block*)mmap(NULL, size,
      PROT_READ | PROT_WRITE,
      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (block == MAP_FAILED)
    return NULL;
  init_block(block, size - META_SIZE);
  if (mem->last)
    mem->last->next_mem = block;
  mem->last = block;
  store_free_block(block);
  return block;
}

void *cook_free_block(struct s_block *block, size_t size)
{
  block = split_block(block, size);
  struct s_block *prev = block->prev_sto;
  if (prev)
  {
    prev->next_sto = block->next_sto;
    if (prev->next_sto)
      prev->next_sto->prev_sto = prev;
  }
  else
  {
    s_memory *memory = get_memory_data();
    memory->free = block->next_sto;
    if (memory->free)
      memory->free->prev_sto = NULL;
  }
  block->prev_sto = NULL;
  block->next_sto = NULL;
  return block + 1;
}
