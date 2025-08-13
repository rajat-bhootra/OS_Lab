.global _start
.extern main
_start:
    la sp, stack_top
    call main
    j .
    .section .bss
    .align 4
    .space 4096

stack_top: