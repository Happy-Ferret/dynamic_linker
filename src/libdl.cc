#include "defines.hh"
#include "elf-manager.hh"
#include "strings.hh"
#include "types.hh"
#include "utils.hh"

void *intern_my_dlopen(const char *filename, int flags)
{
  (void)flags;
  size_t rpos = (size_t)strrpos(filename, '/');
  size_t len = strlen(filename);
  ElfManager *elfmgr = nullptr;

  if (rpos > len)
  {
    char *path = strcat("./", filename);
    elfmgr = new ElfManager(path, filename);
  }
  else
  {
    const char *libname = filename + rpos;
    elfmgr = new ElfManager(filename, libname);
  }

  if (elfmgr && *elfmgr)
    elfmgr->load_lib();
  if (elfmgr && *elfmgr)
    elfmgr->link_dependencies();
  if (!elfmgr || !*elfmgr)
    return nullptr;
  return elfmgr;
}

extern "C"
{
  __attribute__((visibility("default")))
    void *my_dlopen(const char *filename, int flags)
    {
      print("TEST\n\n");
      return intern_my_dlopen(filename, flags);
    }

  __attribute__((visibility("default")))
    void *my_dlsym(void *handle, const char *symbol)
    {
      size_t addr = (size_t)((ElfManager*)handle)->get_symbol_address(symbol);
      void *segm = (void*)((ElfManager*)handle)->get_segment();
      return (void*)((char*)segm + addr);
    }

  __attribute__((visibility("default")))
    const char *my_dlerror(void)
    {
      return "No error";
    }

  __attribute__((visibility("default")))
    int my_dlclose(void *handle)
    {
      (void)handle;
      return 0;
    }
}
