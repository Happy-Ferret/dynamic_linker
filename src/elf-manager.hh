#pragma once

#include "elf.hh"
#include "elf-hash.hh"
#include "vector.hh"
#include "strings.hh"

/* This class help access some fields in an elf.
 *  - Get Dependencies list
 *  - Access Program header
 */
class ElfManager
{
  private:
    bool ready_;
    bool main_;
    bool debug_;
    bool error_;

    char *path_;
    char *name_;

    int fd_;
    void *entry_;
    void *phdr_;
    Elf64_Ehdr *ehdr_;
    Vector<ElfManager*> dependencies_;
    ElfHash* hashmgr_;
    void* segment_;

    Elf64_Addr  get_addr_offset();

    Elf64_Phdr* get_program_header();
    Elf64_Dyn*  get_dynamic_table();

    template<typename T>
    T* get_dynamic_section(Elf64_Sxword tag);
    size_t get_dynamic_section(Elf64_Sxword tag);

    bool load_elf(const char* filename);
    void load_segment();
    void patch_relatable(Elf64_Rela *r, size_t rela_size,
                         Vector<ElfManager*> *deps = nullptr);

  public:
    //Access from file
    ElfManager(const char* filename, const char* name, bool debug = false);
    //Access from auxv
    ElfManager(void *phdr_address);
    ~ElfManager();
    void unload();

    inline char* get_name()    const { return name_; }
    inline char* get_path()    const { return path_; }
    inline void* get_entry()   const { return entry_; }
    inline void* get_segment() const { return segment_; }
    inline int get_fd()        const { return fd_; }

    void load_dependencies(Vector<ElfManager*>& loaded,
                           Vector<String>& viewed);
    void show_dependencies();
    void load_lib();

    void link_dependencies(Vector<ElfManager*> *deps = nullptr);
    void* get_symbol_address(const char* name);

    operator bool() const noexcept { return ready_ && !error_; }
};
