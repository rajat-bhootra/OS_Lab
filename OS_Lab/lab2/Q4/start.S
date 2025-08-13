    .section .text
    .global _start
    .extern kernel_main
    .extern trap_handler

_start:
    la sp, stack_top         # Initialize stack pointer
    la t0, trap_handler      # Load address of trap handler
    csrw mtvec, t0           # Set mtvec to our trap handler
    call kernel_main         # Jump to C kernel main
    j .                      # Should never return

    .section .bss
    .align 4
    .space 4096
stack_top:
