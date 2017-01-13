CC=gcc
CFLAGS=-nostdlib -fPIC -Wall -Werror -Wextra -pedantic -iquote include/
LDFLAG=-nostdlib
LNKFLAG=-Wl, --dynamic-linker=./my_ld.so
VPATH=src/

all:libsyscall.so libstringdum liballocators.so libstring.so \
    libprint.so libprint2.so libprint3.so
	@make clean

libsyscall.so:syscall.o
	$(CC) $< $(LDFLAG) -shared -o $@

libprint.so:print.o
	$(CC) $< $(LDFLAG) -shared -o $@

libprint2.so:print2.o libsyscall.so
	$(CC) $< $(LDFLAG) -shared -o $@ -L. -lsyscall

libprint3.so:print3.o libsyscall.so
	$(CC) $< $(LDFLAG) -shared -o $@ -L. -lsyscall

libstringdum:string.o
	$(CC) $(LDFLAG) string.o -shared -o libstring.so

liballocators.so:allocators_bops.o allocators.o allocators_freeop.o \
                 allocators_global.o allocators_helpers.o
	$(CC) $^ $(LDFLAG) -shared -o $@ -L. -lstring -lsyscall

libstring.so:string.o
	$(CC) $< $(LDFLAG) -shared -o $@ -L. -lallocators

clean:
	$(RM) *.o

clean-all:clean
	$(RM) libsyscall.so
	$(RM) libstring.so
	$(RM) liballocators.so
	$(RM) libprint.so
	$(RM) libprint2.so
	$(RM) libprint3.so
