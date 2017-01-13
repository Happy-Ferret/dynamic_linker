  .section .text
  .globl _start
  .globl ld_main

get_address:
  mov (%rsp), %rax
  ret

_start:
  mov %rsp, %r12      # Save stack in calle saved registers
  mov %rbp, %r13

  movq (%rsp), %rdi   # Setup argc
  mov %rsp, %rsi
  addq $8, %rsi       # Setup argv

  call ld_main

  test %rax, %rax     # If linker failed, or display only, exit(1)
  jz .LEND

  mov %r12, %rsp      # Reset stack et jump entry
  mov %r13, %rbp
  jmp *%rax

.LEND:
  mov $0, %rdi        # Return value setup
  mov $60, %rax
  syscall             # run boy run
