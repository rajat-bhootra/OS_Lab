    .section .text
    .global _start
    .extern kernel_main
    .extern trap_handler

_start:
    la sp, stack_top         # Set stack pointer (stack_top in .bss)
    la t0, trap_handler      # Load trap handler address
    csrw mtvec, t0           # Set mtvec (requires -march=rv32i_zicsr)
    call kernel_main         # Jump to C kernel_main
    j .

    .section .bss
    .align 4
    .space 4096
stack_top:
 