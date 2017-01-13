#include "utils.hh"
#include "types.hh"
#include "defines.hh"
#include "elf-hash.hh"
#include "elf.hh"
#include "utils.hh"
#include "strings.hh"


ElfHash::ElfHash(void *hash_addr, bool hash64, bool gnu_hash)
  : header_((Elf64_Hash*)hash_addr)
  , hash64_(hash64)
{
  gnu_hash_ = gnu_hash;
  n_buck_ = header_->buckets;
  n_mask_ = header_->maskwords;
  mask_ = (Elf64_Xword*)(header_ + 1);
  buck_ = (Elf64_Word*)(mask_ + n_mask_);
  hash_ = (Elf64_Word*)(buck_ + n_buck_);
/*
  print("Shift  (");
  print(ztoa((size_t)header_->shift2));
  print(")\n");

  print("Buckets  (");
  print(ztoa(n_buck_));
  print(")\n");
  for (size_t i = 0; i < n_buck_; i++, print("\t"))
    print(ztoa((size_t)buck_[i]));

  print("\nMasks  (");
  print(ztoa(n_mask_));
  print(")\n");
  for (size_t i = 0; i < n_mask_; i++, print("\t"))
    print(ztoa((size_t)mask_[i]));

  print("\nHash (");
  print(ztoa(n_buck_));
  print(")\n");
  for (size_t i = 0; i < n_buck_; i++, print("\t"))
    print(ztoa((size_t)hash_[i]));
  print("\n");
*/
}

ElfHash::~ElfHash()
{}

#define USE_BLOOM 0
Elf64_Sym* ElfHash::lookup_symbol(const char* name, Elf64_Sym *symtab,
                                  const char* strtab)
{
  if (!gnu_hash_)
    return lookup_symbol_SV(name, symtab, strtab);
  Elf64_Word hash_1 = hash_symbol(name);
  Elf64_Word hash_2 = hash_1 >> header_->shift2;
  Elf64_Word c = (hash64_ ? sizeof (Elf64_Xword) : sizeof (Elf64_Word)) * 8;
  Elf64_Word n = (hash_1 / c) & header_->maskwords;
  Elf64_Word bitmask = (1 << (hash_1 % c)) | (1 << (hash_2 % c));

#if USE_BLOOM
  if ((mask_[n] & bitmask) != bitmask)
    return nullptr;
#else
    (void)bitmask;
#endif

  n = buck_[hash_1 % n_buck_];
  if (n == 0)
    return nullptr;

  Elf64_Sym* sym = &symtab[n];
  Elf64_Word *hashval = &hash_[n - header_->symndx];

  for (hash_1 &= ~1; 1; sym++)
  {
    hash_2 = *hashval++;
    if ((hash_1 = (hash_2 & ~1))
      && !strcmp(name, strtab + sym->st_name))
      return sym;
    if (hash_2 & 1)
      break;
  }

  return nullptr;
}

Elf64_Sym* ElfHash::lookup_symbol_SV(const char *name,
                                     Elf64_Sym *symtab,
                                     const char* strtab)
{
  size_t len = ((char*)strtab - (char*)symtab) / sizeof (Elf64_Sym);
  for (size_t i = 1; i < len; i++)
    if (!strcmp(name, symtab[i].st_name + strtab))
      return symtab + i;
  return nullptr;
}

Elf64_Word ElfHash::hash_symbol(const char *name)
{
  Elf64_Word hash = 5381;
  for (unsigned char c = *name; c; c = *(++name))
    hash = hash * 33 + c;
  return hash;
}
