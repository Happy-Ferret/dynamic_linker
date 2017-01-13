#include "libmalloc.hh"

extern "C"
{
  void store_free_block(struct s_block *block)
  {
    struct s_memory *mem = get_memory_data();
    struct s_block *current = mem->free;
    if (!mem->free)
    {
      block->next_sto = nullptr;
      block->prev_sto = nullptr;
      mem->free = block;
    }
    else if (block < mem->free)
    {
      block->next_sto = mem->free;
      block->prev_sto = nullptr;
      mem->free = block;
      block->next_sto->prev_sto = block;
    }
    else
    {
      while (current->next_sto && current->next_sto < block)
        current = current->next_sto;
      block->next_sto = current->next_sto;
      block->prev_sto = current;
      current->next_sto = block;
      if (block->next_sto)
        block->next_sto->prev_sto = block;
    }
  }

  struct s_block *get_free_block(size_t size)
  {
    struct s_memory *mem = get_memory_data();
    if (!mem->free)
      return nullptr;
    struct s_block *free = mem->free;
    for (; free; free = free->next_sto)
      if (free->capacity >= size)
        break;
    return free;
  }
}
