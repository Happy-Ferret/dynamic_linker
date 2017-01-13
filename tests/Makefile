CC=gcc
CFLAGS=-Wall -Wextra -nostdlib -pedantic -fno-builtin -std=c99        \
       -L. -lsyscall -lprint -lstring -lallocators -lprint2 -lprint3  \
       -L../ -ldl_my                                                  \
       -nostartfiles -iquote include -g
LNKFLAG=-Wl,--dynamic-linker=./../my_ld.so

all:bin

bin:loader.o
	$(CC) $(CFLAGS) $(LNKFLAG) check.c loader.o -o check

check:bin
	LD_LIBRARY_PATH=. ./check

clean:
	$(RM) check
