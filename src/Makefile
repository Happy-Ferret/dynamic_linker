CXX=g++
CXXFLAGS=-Wall -Werror -Wextra -O0 -g -std=c++14 -fPIC -nostdinc \
         -ffreestanding -fno-exceptions -fvisibility=hidden -iquote ./ \
         -fno-builtin -nostdlib -fno-builtin-function -nodefaultlibs \
         -fno-rtti

VPATH=./malloc ./

main_CXXFLAGS=-Wall -Werror -Wextra -O0 -g -std=c++14


MALLOC_FNAME=block_ops.cc calloc.cc free.cc free_ops.cc global.cc helpers.cc malloc.cc realloc.cc
SRC_MALLOC=$(addprefix malloc/,$(MALLOC_FNAME))
OBJ_MALLOC=$(SRC_MALLOC:.cc=.o)

SRC=main.cc elf-manager.cc strings.cc syscall.cc utils.cc elf-hash.cc
OBJ=$(SRC:.cc=.o)


all:my_ld.so libdl_my.so



my_ld.so:$(OBJ) $(OBJ_MALLOC)
	$(CXX) strap.s -fPIC -c -fvisibility=hidden
	ld $(OBJ) $(OBJ_MALLOC) strap.o -shared -o my_ld.so
	cp my_ld.so ../my_ld.so

libdl_my.so:$(OBJ) $(OBJ_MALLOC) libdl.o
	$(CXX) strap.s -fPIC -c -fvisibility=hidden
	ld $(OBJ) $(OBJ_MALLOC) libdl.o strap.o -shared -o libdl_my.so
	cp libdl_my.so ../libdl_my.so

clean:
	$(RM) $(OBJ) libdl_my.so strap.o ../my_ld.so $(OBJ_MALLOC) *.lo
	$(RM) libdl_my.so my_ld.so libdl.o ../libdl_my.so
