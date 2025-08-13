.extern main
.extern trap_handler
.global _start
_start:
    la sp, stack_top
    la t0, trap_handler
    csrw mtvec, t0
    call main
    j .
    .section .bss
    .align 4
    .space 4096
    
stack_top: