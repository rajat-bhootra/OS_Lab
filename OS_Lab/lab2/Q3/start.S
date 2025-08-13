.global _start
.section .text
_start:
    la sp, stack_top      # Set stack pointer
    call kernel_main      # Call C kernel
    j .
    .section .bss
    .align 12
    .space 4096
stack_top:
