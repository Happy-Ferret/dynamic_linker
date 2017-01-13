#pragma once

//general
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define NULL ((void*)0)

// Open
#define O_RDONLY 0
#define O_WRONLY 1

// lseek
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2


// mmap
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define PROT_NONE 0x0
#define PROT_GROWSDOWN 0x01000000

#define MAP_SHARED 0x01
#define MAP_PRIVATE 0x02
#define MAP_ANONYMOUS 0x20

#define MAP_FAILED ((void*)-1)
