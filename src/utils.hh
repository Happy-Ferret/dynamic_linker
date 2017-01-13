#pragma once

#include "elf.hh"
#include "types.hh"
#include "variadic.hh"

void *forward_ptr(void *ptr, size_t offset);
void fputs(int fd, const char* text);
bool check_magic(const unsigned char m[EI_NIDENT]);

//new
extern "C" void *_Znwm(unsigned long size);
//delete
extern "C" void _ZdlPvm(void *ptr, unsigned long size);
extern "C" void _ZdlPv(void *ptr);

void print(const char *str);
void print(const char *a, const char *b);
void print(const char *a, const char *b, const char* c);
void print(const char *a, const char *b, const char* c, const char* d);
void print(const char *a, const char *b, const char* c,
           const char* d, const char* e);
void print(const char *a, const char *b, const char* c,
           const char* d, const char* e, const char *f);
void print(const char *a, const char *b, const char* c,
           const char* d, const char* e, const char *f, const char *g);

void printf(const char* format, ...);

char* ztoa16(unsigned long n);
char* ztoa(unsigned long n);
char* getenv(char** env, const char *name);
