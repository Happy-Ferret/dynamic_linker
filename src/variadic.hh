#pragma once

#include "types.hh"

typedef struct
{
  void *start = nullptr;
  size_t got = 0;
  size_t data_A = 0;
  size_t data_B = 0;
  size_t data_C = 0;
} va_arg;

#define VA_START()                                  \
  va_arg __va_data =                                \
  {                                                 \
    .start = nullptr,                               \
    .got = 0,                                       \
    .data_A = 0,                                    \
    .data_B = 0,                                    \
    .data_C = 0,                                    \
  };                                                \
  __asm volatile ( "nop" : "=S"(__va_data.data_A)); \
  __asm volatile ( "nop" : "=d"(__va_data.data_B));
  /*
  __asm volatile ( "push %%rsi" );                  \
  __asm volatile ( "mov %%r10, %%rsi"               \
                   : "=S"(__va_data.data_C));       \
  __asm volatile ( "pop %%rsi" );
*/

#define VA_NEXT(vdata, Type)                     \
  get_argument<Type>(&__va_data);

template <typename T>
T get_argument(va_arg *vdata);

#include "variadic.hxx"
