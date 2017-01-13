#pragma once

#include "types.h"

void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);
void *malloc(size_t size);
void free(void *ptr);
