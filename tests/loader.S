.section .text
.globl _start
.globl mwrite
.globl main

_start:
  movq (%rsp), %rdi
  addq $8, %rsp
  mov %rsp, %rsi

  mov %rsp, %rbp
  push %rax
  push %rsp

  call main

  mov %rax, %rdi
  mov $60, %rax
  syscall

mwrite:
  push %rbp
  mov %rsp, %rbp

  mov $1, %eax
  syscall

  leave
  ret
