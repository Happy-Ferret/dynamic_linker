#pragma once

#include "defines.hh"
#include "libmalloc.hh"
#include "types.hh"

template<typename T>
class Vector
{
  private:
    size_t capacity_;
    size_t size_;
    T *array_;

  public:
    Vector(size_t size);
    ~Vector();

    T& operator[](size_t index);
    void push_back(T elt);
    bool contains(T elt);

    size_t get_size() const noexcept  { return size_; }
};

#include "vector.hxx"
