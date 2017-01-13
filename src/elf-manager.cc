#include "elf.hh"

#include "defines.hh"
#include "elf-hash.hh"
#include "elf-manager.hh"
#include "elf.hh"
#include "strings.hh"
#include "vector.hh"
#include "syscall.hh"
#include "utils.hh"

ElfManager::ElfManager(const char* filename, const char* name, bool debug)
  : ElfManager((void*)nullptr)
{
  path_ = strdup(filename);
  name_ = strdup(name);
  debug_ = debug;

  ready_ = load_elf(filename);
  if (ready_)
    load_segment();
}

ElfManager::ElfManager(void *phdr_address)
  : ready_(false)
  , main_(phdr_address != nullptr)
  , debug_(false)
  , fd_(-1)
  , entry_(nullptr)
  , phdr_(phdr_address)
  , ehdr_(nullptr)
  , dependencies_(Vector<ElfManager*>(2))
{
  ready_ = true;
}

bool ElfManager::load_elf(const char* filename)
{
  fd_ = open(filename, O_RDONLY);
  if (fd_ < 0)
  {
    print("\033[91mElfManager: Unable to open the file : '",
           filename, "'\n\033[0m");
    error_ = 2;
    return false;
  }

  lseek(fd_, 0, SEEK_END);
  off_t file_size = lseek(fd_, 0, SEEK_CUR);
  lseek(fd_, 0, SEEK_SET);
  entry_ = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd_, 0);
  if (!entry_ || file_size < static_cast<off_t>(sizeof(Elf64_Ehdr)))
    return false;

  ehdr_ = static_cast<Elf64_Ehdr*>(entry_);
  if (!check_magic(ehdr_->e_ident))
  {
    fputs(STDERR, "Magic not matching.");
    fputs(STDERR, filename);
    error_ = 3;
    return false;
  }
  get_program_header();
  return true;
}

void ElfManager::unload()
{
  if (entry_ && !main_)
  {
    lseek(fd_, 0, SEEK_END);
    off_t file_size = lseek(fd_, 0, SEEK_CUR);
    munmap(entry_, file_size);
  }

  if (fd_ >= 0)
    close(fd_);
}

ElfManager::~ElfManager()
{
  unload();
}

Elf64_Phdr* ElfManager::get_program_header()
{
  if (!phdr_)
  {
    void *ptr = forward_ptr(entry_, ehdr_->e_phoff);
    phdr_ = static_cast<Elf64_Phdr*>(ptr);
  }
  return (Elf64_Phdr*)phdr_;
}

Elf64_Addr ElfManager::get_addr_offset()
{
  if (main_)
    return 0;
  Elf64_Phdr *phdr = get_program_header();
  return phdr->p_vaddr - (phdr->p_offset + (unsigned long)entry_);
}

Elf64_Dyn *ElfManager::get_dynamic_table()
{
  Elf64_Phdr *phdr = get_program_header();
  for (; phdr->p_type != PT_NULL; phdr++)
  {
    if (phdr->p_type == PT_DYNAMIC)
    {
      if (!main_)
      {
        char* ptr = static_cast<char*>(entry_);
        ptr += phdr->p_offset;
        return static_cast<Elf64_Dyn*>(static_cast<void*>(ptr));
      }
      else
        return (Elf64_Dyn*)phdr->p_vaddr;
    }
  }
  return nullptr;
}

template <typename T>
T* ElfManager::get_dynamic_section(Elf64_Sxword tag)
{
  Elf64_Dyn* dyn = get_dynamic_table();
  if (!dyn)
    return nullptr;
  Elf64_Addr offset = get_addr_offset();
  for (; dyn->d_tag != DT_NULL; dyn++)
  {
    if (dyn->d_tag == tag)
      return (T*)(dyn->d_un.d_ptr - offset);
  }
  return nullptr;
}

size_t ElfManager::get_dynamic_section(Elf64_Sxword tag)
{
  Elf64_Dyn* dyn = get_dynamic_table();
  if (!dyn)
  {
    fputs(STDERR, "[Error] No dynamic segment");
    return 0;
  }
  for (; dyn->d_tag != DT_NULL; dyn++)
    if (dyn->d_tag == tag)
      return (size_t)(dyn->d_un.d_val);
  return 0;
}

const char* TMP_get_path(unsigned int i)
{
  if (i == 0) return "./";
  else if (i == 1) return "/usr/lib64/";
  else if (i == 2) return "/usr/lib/";
  return "Unknown";
}

const char* find_libpath(const char *libname)
{
  for (int i = 0; i < 3; i++)
  {
    int fd = open(strcat(TMP_get_path(i), libname), O_RDONLY);
    if (fd >= 0)
    {
      close(fd);
      return TMP_get_path(i);
    }
  }
  return "./";
}


void ElfManager::load_dependencies(Vector<ElfManager*>& loaded,
                                   Vector<String>& viewed)
{
  Elf64_Dyn *dyn = get_dynamic_table();
  char *strtab = get_dynamic_section<char>(DT_STRTAB);
  if (!dyn || !strtab)
  {
    fputs(STDERR, "[Error] Cannot read .dynamic section");
    error_ = 4;
    return;
  }

  Vector<ElfManager*> to_append(2);
  for (; dyn->d_tag != DT_NULL; dyn++)
  {
    if (dyn->d_tag == DT_NEEDED)
    {
      char* name = strtab + dyn->d_un.d_ptr;
      if (viewed.contains(name))
        break;
      viewed.push_back(String(name));
      char *p = get_dynamic_section<char>(DT_RPATH);
      (void)p;

      ElfManager *elfmgr = new ElfManager(strcat(find_libpath(name), name),
                                          strdup(name));
      loaded.push_back(elfmgr);
      to_append.push_back(elfmgr);
    }
  }

  for (size_t i = 0; i < loaded.get_size(); i++)
    if (to_append[i])
      to_append[i]->load_dependencies(loaded, viewed);
  if (main_ || debug_)
    dependencies_ = loaded;
}

void ElfManager::show_dependencies()
{
  for (size_t i = 0; i < dependencies_.get_size(); i++)
  {
    if (dependencies_[i])
    {
      print("\t", dependencies_[i]->get_name());
      if (dependencies_[i]->get_fd() != -1)
        print(" => ", dependencies_[i]->get_path());
      print(" (0x", ztoa16((Elf64_Addr)dependencies_[i]->get_segment()),
            ")\n");
    }
  }
}

void ElfManager::load_segment()
{
  Elf64_Phdr *phdr = get_program_header();
  Elf64_Phdr *load1 = nullptr;
  Elf64_Phdr *load2 = nullptr;
  for (; phdr->p_type != PT_NULL; phdr++)
  {
    if (phdr->p_type == PT_LOAD)
    {
      if (!load1)
        load1 = phdr;
      else
      {
        load2 = phdr;
        break;
      }
    }
  }
  if (!load1 || !load2)
  {
    fputs(STDERR, "Unable to map the library.");
    error_ = 5;
    return;
  }


  size_t size = load2->p_vaddr + load2->p_filesz;
  segment_ = mmap(nullptr, size, load2->p_flags | load1->p_flags,
                  MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
  void* write_loc1 = (char*)segment_;
  void* write_loc2 = load2->p_vaddr + (char*)segment_;

  memcpy(write_loc1, (void*)((char*)entry_ + load1->p_offset),
         load1->p_filesz);
  memcpy(write_loc2, (void*)((char*)entry_ + load2->p_offset),
         load2->p_filesz);
}

void ElfManager::link_dependencies(Vector<ElfManager*> *deps)
{
  Elf64_Rela *rela = get_dynamic_section<Elf64_Rela>(DT_RELA);
  if (rela)
  {
    size_t size = get_dynamic_section(DT_RELASZ) / sizeof (Elf64_Rela);
    print("\n\033[1m[DT_RELA]\033[0m (", ztoa(size), " symbols).\n");
    patch_relatable(rela, size, deps);
  }

  Elf64_Rela *plt = get_dynamic_section<Elf64_Rela>(DT_JMPREL);
  if (plt)
  {
    size_t size = get_dynamic_section(DT_PLTRELSZ) / sizeof (Elf64_Rela);
    print("\n\033[1m[DT_PLTREL]\033[0m (", ztoa(size), " symbols).\n");
    patch_relatable(plt, size, deps);
  }
}

void ElfManager::patch_relatable(Elf64_Rela *r, size_t rela_size,
                                 Vector<ElfManager*> *deps)
{
  Elf64_Sym  *s = get_dynamic_section<Elf64_Sym>(DT_SYMTAB);
  char *str = get_dynamic_section<char>(DT_STRTAB);

  for (size_t i = 0; i < rela_size; i++)
  {
    void* address = nullptr;
    if (name_)
      print("[", name_,"][REL][");
    else
      print("[--][REL][");
    Elf64_Xword sym =  ELF64_R_SYM(r[i].r_info);
    print("[ID=", ztoa(sym), "]");
    Elf64_Xword type = ELF64_R_TYPE(r[i].r_info);
    (void)sym;


    if (type == R_SH_NONE)
    { print("[NOP]", str + s[sym].st_name, "\n"); return; }
    else if (type == R_X86_64_JUMP_SLOT) { print("[JMP] "); }
    else if (type == R_X86_64_TPOFF64) { print("[O64] "); }
    else if (type == R_X86_64_RELATIVE) { print("[REL] "); }
    else { print("[UKN ", ztoa(type) ,"]"); }

    Vector<ElfManager*> *d = deps ? deps : &dependencies_;

    for (size_t j = 0; !address && j < d->get_size(); j++)
    {
      void* addr = (*d)[j]->get_symbol_address(str + s[sym].st_name);
      if (addr == nullptr)
        continue;
      print("\033[92m[LNK]\033[0m symbol ", str + s[sym].st_name,
            " in ", (*d)[j]->get_name(), "@");
      print("0x", ztoa16((size_t)addr), "\n");

      if ((*d)[j]->get_name() != name_)
      {
        (*d)[j]->link_dependencies(d);
      }
      void* segm = (*d)[j]->get_segment();
      address = (void*)((size_t)addr + (size_t)segm);
    }

    if (address)
    {
      void *write_addr = segment_;
      write_addr = forward_ptr(write_addr, r[i].r_offset);
      *(Elf64_Addr*)write_addr = (Elf64_Addr)address;
    }
    else
    {
      print("\033[91m[LNK]\033[0m ", str + s[sym].st_name, " not found.\n");
      error_ = true;
    }
  }
  print("\n");
}

void* ElfManager::get_symbol_address(const char* name)
{
  Elf64_Sym *symt = get_dynamic_section<Elf64_Sym>(DT_SYMTAB);
  char *strt      = get_dynamic_section<char>(DT_STRTAB);
  if (!strt || !symt)
    return nullptr;

  if (!hashmgr_)
  {
    Elf64_Hash *gnu_hash = get_dynamic_section<Elf64_Hash>(DT_GNU_HASH);
    Elf64_Hash *hash = get_dynamic_section<Elf64_Hash>(DT_HASH);
    if (!gnu_hash && !hash)
    {
      fputs(STDERR, "No hash table whatsoever...");
      return nullptr;
    }
    hashmgr_ = new ElfHash((void*)(hash ? hash : gnu_hash), false,
                           (bool)gnu_hash);
  }
  if (!hashmgr_)
    return nullptr;

  Elf64_Sym *sym = hashmgr_->lookup_symbol(name, symt, strt);
  if (!sym)
    return nullptr;
  return (void*)(sym->st_value);
}

void ElfManager::load_lib()
{
  Vector<ElfManager*> loaded = Vector<ElfManager*>(2);
  Vector<String> viewed = Vector<String>(2);
  load_dependencies(loaded, viewed);
}
