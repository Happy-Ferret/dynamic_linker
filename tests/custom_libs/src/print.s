.section .text
.globl print_some
.type print_some, @function

print_some:
  push %rbp
  mov %rsp, %rbp

  mov $1, %eax
  syscall

  leave
  ret
.size print_some, .-print_some

