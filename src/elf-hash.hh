#pragma once

#include "elf.hh"

class ElfHash
{
  private:
    Elf64_Hash* header_;
    bool gnu_hash_;
    bool hash64_;
    size_t n_buck_;
    size_t n_mask_;
    Elf64_Xword* mask_;
    Elf64_Word* buck_;
    Elf64_Word* hash_;

    static Elf64_Word hash_symbol(const char *name);
    Elf64_Sym* lookup_symbol_SV(const char *name,
                                         Elf64_Sym *symtab,
                                         const char* strtab);

  public:
    ElfHash(void* hash_addr, bool hash64, bool gnu_hash);
    ~ElfHash();
    Elf64_Sym* lookup_symbol(const char* name, Elf64_Sym* symtab,
                        const char* strtab);

};
