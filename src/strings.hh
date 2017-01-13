#pragma once

#include "types.hh"
#include "defines.hh"


size_t strlen(const char* str);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, size_t len);

size_t strpos(const char *str, char c);
int strrpos(const char *str, char c);
char* substr(const char *str, size_t start, size_t len);

void *memset(void *s, int c, size_t size);
void *memcpy(void *dest, void *src, size_t n);
char *strcat(const char *a, const char *b);
char *strdup(const char *in);

class String
{
  private:
    char *cstr_;

  public:
    String(char* str);
    const char* get_cstr() const { return cstr_; }
};
bool operator==(String a, String b);
