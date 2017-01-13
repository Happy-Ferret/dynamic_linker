.section .text
.globl _start
.type _start, @function
_start:
  movq (%rsp), %rdi
  addq $8, %rsp
  mov %rsp, %rsi

  mov %rsp, %rbp
  push %rax
  push %rsp

  call main@PLT

  mov %rax, %rdi
  mov $60, %rax
  syscall
.size _start, .-_start
