set breakpoint pending yes
layout src
winheight src -5
focus cmd

define printblk
  set $f = (s_block*)((s_memory*)memory)->blocks
  while $f
    printf "%p [%u] \t -> %p\n", $f, $f->capacity, $f->next_mem
    set $f = ($f)->next_mem
  end
end


define printfree
  printf "Free\n"
  set $f = (s_block*)((s_memory*)memory)->free
  while $f
    printf "%p [%u] \t -> %p\n", $f, $f->capacity, $f->next_mem
    set $f = $f->next_sto
  end
end

define printmem
  printf "Memory\n"
  printblk
  printfree
end

define dodebug
  c
  p ptr
  printmem
end

define print_symbols
  set $f = get_dynamic_section<Elf64_Sym>(6)
  set $s = get_dynamic_section<size_t>(11)
  while $f
    printf ""
  end
end

define print_env
  set $i = 0
  while *(env + $i)
    printf "%s\n", *(env + $i)
    set $i = $i + 1
  end
end

define p_sym
  printf "Symbol: %s\n", str + s[i].st_name
end

define p_dep
  printf "Dependence: %s\n", dependencies_[j]->get_name()
end

#set environment LD_TRACE_LOADED_OBJECTS 1


#b malloc.cc:19
#run
#p ptr
#printmem
#b elf-manager.cc:209
#b ld_main
#b ElfManager::patch_relatable
#layout asm
layout asm
run a b
si
