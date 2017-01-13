#include "libmalloc.hh"
#include "syscall.hh"
#include "defines.hh"

void *memory = 0;

extern "C"
{
  /*
  struct s_memory memory =
  {
    .page_size = 0,
    .blocks = nullptr,
    .last = nullptr,
    .free = nullptr,
  };

  struct s_memory *get_memory_data()
  {
    if (memory.blocks == nullptr)
    {
      memory.page_size = PAGE_SIZE;
      struct s_block *block = (s_block*)mmap(nullptr, PAGE_SIZE,
                     PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
      init_block(block, PAGE_SIZE - META_SIZE);
      memory.blocks = block;
      memory.last = block;
      memory.free = block;
    }
    return &memory;
  }
  */
  struct s_memory *get_memory_data()
  {
    if (memory)
      return (s_memory*)memory;
    memory = (s_memory*)mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
                            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    struct s_memory *mem = (s_memory*)memory;

    s_block *block = (s_block*)mmap(nullptr, PAGE_SIZE, PROT_READ | PROT_WRITE,
                                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    init_block(block, PAGE_SIZE - META_SIZE);
    mem->blocks = block;
    mem->last = block;
    mem->free = block;
    return mem;
  }
}
