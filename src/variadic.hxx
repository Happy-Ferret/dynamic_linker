#include "variadic.hh"
#include "syscall.hh"

template <typename T>
T get_argument(va_arg *vdata)
{
  vdata->got++;
  if(vdata->got == 1)
    return (T)(vdata->data_A);
  if(vdata->got == 2)
    return (T)(vdata->data_B);
  if(vdata->got == 3)
    return (T)(vdata->data_C);
  else
    write(1, "nope\n", 4);

  return (T)nullptr;
}
