#include "defines.hh"
#include "elf-manager.hh"
#include "elf.hh"
#include "strings.hh"
#include "syscall.hh"
#include "types.hh"
#include "utils.hh"
#include "strings.hh"
#include "vector.hh"
#include "libmalloc.hh"

void *get_auxv(char** argv)
{
  while (*argv) //Skip Args
    argv++;
  argv++;
  while (*argv) //Skip Env
    argv++;
  argv++;
  return argv;
}

char** get_env(char **argv)
{
  while (*argv)
    argv++;
  return argv + 1;
}

void* read_auxv(Elf64_auxv_t* auxv, uint64_t type)
{
  for (; auxv->a_type != AT_NULL; auxv++)
    if (auxv->a_type == type)
      return (void*)auxv->a_un.a_ptr;
  return nullptr;
}

extern "C"
{
  int debug_main(int argc, char** argv);

  int ld_main(int argc, char** argv)
  {
    if (argc == 2)
      return debug_main(argc, argv);
    char **env = get_env(argv);
    Elf64_auxv_t *auxv = static_cast<Elf64_auxv_t*>(get_auxv(argv));
    void *phdr = read_auxv(auxv, AT_PHDR);
    void *entry = read_auxv(auxv, AT_ENTRY);
    (void)argc;

    ElfManager elfmgr = ElfManager(phdr);
    if (!elfmgr)
      return 2;

    Vector<ElfManager*> loaded = Vector<ElfManager*>(2);
    Vector<String> viewed = Vector<String>(2);
    elfmgr.load_dependencies(loaded, viewed);


    char* trace_loaded = getenv(env, "LD_TRACE_LOADED_OBJECTS");
    if (trace_loaded && !strcmp(trace_loaded, "1"))
      elfmgr.show_dependencies();
    else
    {
      elfmgr.link_dependencies();
      if (elfmgr)
      {
        print("Jump to 0x", ztoa16((size_t)entry), "\n================\n");
        return (size_t)entry;
      }
      fputs(STDERR, "[ERROR] Unable to link some symbols");
    }

    if (elfmgr)
      elfmgr.unload();
    return 0;
  }

  int debug_main(int argc, char** argv)
  {
    char **env = get_env(argv);
    (void)env;
    (void)argc;

    size_t rpos = (size_t)strrpos(argv[1], '/');
    size_t len = strlen(argv[1]);
    const char *libname = argv[1] + rpos;

    ElfManager *elfmgr = nullptr;

    if (rpos < len)
      elfmgr = new ElfManager(argv[1], libname, true);
    else
      elfmgr = new ElfManager(argv[1], argv[1], true);

    if (!elfmgr || !*elfmgr)
      return 0;

    Vector<ElfManager*> loaded = Vector<ElfManager*>(2);
    Vector<String> viewed = Vector<String>(2);
    elfmgr->load_dependencies(loaded, viewed);
    elfmgr->show_dependencies();
    return 0;
  }
}
