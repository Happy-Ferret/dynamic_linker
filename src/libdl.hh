#pragma once

#include "types.hh"

void *my_dlopen(const char *filename, int flags);
void *my_dlsym(void *handle, const char *symbol);
const char *my_dlerror(void);
int my_dlclose(void *handle);
