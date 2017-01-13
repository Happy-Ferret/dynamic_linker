#include "vector.hh"
#include "types.hh"
#include "defines.hh"
#include "utils.hh"
#include "utils.hh"

template<typename T>
Vector<T>::Vector(size_t size)
  : capacity_(size < 2 ? 2 : size)
  , size_(0)
  , array_(nullptr)
{
  array_ = (T*)malloc(sizeof (T) * capacity_);
}

template<typename T>
Vector<T>::~Vector()
{
  free(array_);
}

template<typename T>
T& Vector<T>::operator[](size_t index)
{
  return array_[index];
}

template<typename T>
void Vector<T>::push_back(T elt)
{
  (void)elt;
  if (size_ + 1 == capacity_)
  {
    array_ = (T*)realloc(array_, capacity_ * 2 * sizeof (T));
    capacity_ *= 2;
  }
  array_[size_++] = elt;
}

template<typename T>
bool Vector<T>::contains(T elt)
{
  for (size_t i = 0; i < size_; i++)
    if (array_[i] == elt)
      return true;
  return false;
}
