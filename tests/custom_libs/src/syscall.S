.globl read
.globl write
.globl open
.globl close
.globl mmap
.globl munmap
.globl brk
.globl mremap
.globl pipe
.globl dup
.globl dup2
.globl fork
.globl exit

.type read, @function
.type read, @function
read:
  push %rbp
  mov %rsp, %rbp
  mov $0, %eax
  syscall
  leave
  ret
.size read, .-read

.type write, @function
write:
  push %rbp
  mov %rsp, %rbp
  mov $1, %eax
  syscall
  leave
  ret
.size write, .-write

.type open, @function
open:
  push %rbp
  mov %rsp, %rbp
  mov $2, %eax
  syscall
  leave
  ret
.size open, .-open

.type close, @function
close:
  push %rbp
  mov %rsp, %rbp
  mov $3, %eax
  syscall
  leave
  ret
.size close, .-close

.type lseek, @function
lseek:
  push %rbp
  mov %rsp, %rbp
  mov $8, %eax
  syscall
  leave
  ret
.size lseek, .-lseek

.type mmap, @function
mmap:
  push %rbp
  mov %rsp, %rbp
  mov %rcx, %r10
  mov $9, %eax
  syscall
  leave
  ret
.size mmap, .-mmap

.type munmap, @function
munmap:
  push %rbp
  mov %rsp, %rbp
  mov $11, %eax
  syscall
  leave
  ret
.size munmap, .-munmap

.type brk, @function
brk:
  push %rbp
  mov %rsp, %rbp
  mov $12, %eax
  syscall
  leave
  ret
.size brk, .-brk

.type mremap, @function
mremap:
  push %rbp
  mov %rsp, %rbp
  mov %rcx, %r10
  mov $25, %eax
  syscall
  leave
  ret
.size mremap, .-mremap

.type pipe, @function
pipe:
  push %rbp
  mov %rsp, %rbp
  mov $22, %eax
  syscall
  leave
  ret
.size pipe, .-pipe

.type dup, @function
dup:
  push %rbp
  mov %rsp, %rbp
  mov $32, %eax
  syscall
  leave
  ret
.size dup, .-dup

.type dup2, @function
dup2:
  push %rbp
  mov %rsp, %rbp
  mov $33, %eax
  syscall
  leave
  ret
.size dup2, .-dup2

.type fork, @function
fork:
  push %rbp
  mov %rsp, %rbp
  mov $57, %eax
  syscall
  leave
  ret
.size fork, .-fork

.type exit, @function
exit:
  push %rbp
  mov %rsp, %rbp
  mov $60, %eax
  syscall
  leave
  ret
.size exit, .-exit
