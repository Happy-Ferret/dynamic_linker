#pragma once

#include "defines.hh"
#include "types.hh"

extern "C"
{
  struct s_block
  {
    int magic_a;
    size_t capacity;
    struct s_block *next_mem;
    struct s_block *next_sto;
    struct s_block *prev_sto;
    int magic_b;
  };

  struct s_memory
  {
    size_t page_size;
    struct s_block *blocks;
    struct s_block *last;
    struct s_block *free;
  };

# define IS_FREE(Block) \\
        ((next_mem - 0) & 1lu)

# define MIN_BLOCK_SIZE 64
# define PAGE_SIZE (4096)
# define META_SIZE (sizeof (struct s_block))

  //Public
  void *calloc(size_t number, size_t size);
  void *realloc(void *ptr, size_t size);
  void *malloc(size_t size);
  void free(void *ptr);

  //Helpers
  size_t align(size_t value, size_t divisor);
  void init_block(struct s_block *b, size_t size);
  int is_adjacent(struct s_block *block, struct s_block *next);
  void *forward_ptr_c(void *ptr, size_t offset);

  //Globals
  struct s_memory *get_memory_data();

  //Blocks Management
  struct s_block *split_block(struct s_block *block, size_t size);
  void merge_block(struct s_block *block);
  void store_free_block(struct s_block *block);
  struct s_block *get_free_block(size_t size);
  struct s_block *create_new_block(size_t size);
  void *cook_free_block(struct s_block *block, size_t size);
}
