EXTRA_DIST = autogen.sh
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}
AM_MAKEFLAGS = --no-print-directory

AM_CPPFLAGS = -include $(top_builddir)/config.h        \
              -I${top_srcdir}/src -D__DEBUG=0

AM_CXXFLAGS = $(my_flags)

bin_PROGRAMS = my_ld.so libdl_my.so


my_ld_so_LDFLAGS = -shared -fPIC
my_ld_so_SOURCES =        \
  src/elf-manager.cc      \
  src/elf-hash.cc         \
  src/main.cc             \
  src/malloc/block_ops.cc \
  src/malloc/calloc.cc    \
  src/malloc/free.cc      \
  src/malloc/free_ops.cc  \
  src/malloc/global.cc    \
  src/malloc/helpers.cc   \
  src/malloc/malloc.cc    \
  src/malloc/realloc.cc   \
  src/strings.cc          \
  src/strap.s             \
  src/syscall.cc          \
  src/utils.cc

libdl_my_so_LDFLAGS = -shared -fPIC
libdl_my_so_SOURCES =    \
  $(my_ld_so_SOURCES)    \
  src/libdl.cc

#include_HEADERS=          \
#  src/libdl.hh

include_internal=         \
  src/defines.hh          \
  src/elf.hh              \
  src/elf-manager.hh      \
  src/elf-hash.hh         \
  src/libmalloc.hh        \
  src/strings.hh          \
  src/syscall.hh          \
  src/types.hh            \
  src/utils.hh            \
  src/vector.hh           \
  src/vector.hxx


# Tests to run
#TESTS = check

# Test sources
#check_SOURCES = tests/check.cc

# We do not want to install test-related files don't we ?
#noinst_PROGRAMS = check

RM=rm -rf

clean:
	$(RM) aclocal.m4 configure config.h config.h.in autom4te.cache
	$(RM) config.log stamp-h1 build-aux/ config.status Makefile.in
	$(RM) config.h.in~ m4 libtool my_ld.la libdl_my.la
	$(RM) src/.libs src/malloc/.libs
	$(RM) src/.dirstamp src/malloc/.dirstamp
	$(RM) src/.deps src/malloc/.deps .deps
	$(RM) *.o src/*.o malloc/*.o
	@cd tests && make clean && cd custom_libs && make clean && cd .. && cd ..

distclean:clean
	$(RM) $(bin_PROGRAMS) Makefile
	@cd tests/custom_libs && make clean-all && cd .. && cd ..

check:all
	cd tests/custom_libs/ && make && cd .. && make check
