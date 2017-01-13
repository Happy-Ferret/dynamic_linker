#pragma once

#include "types.h"

char *strcat(const char *a, const char *b);
char *strchr(const char *str, int c);
char *strdup(const char *in);
char* substr(const char *str, size_t start, size_t len);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t len);
size_t strlen(const char* str);
size_t strpos(const char *str, char c);
void *memcpy(void *dest, void *src, size_t n);
void *memset(void *s, int c, size_t size);
