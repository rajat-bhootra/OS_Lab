.global _start 
.extern main
.extern trap_handler

_start: 
        la sp, stack_top
        la t0, trap_handler
        csrw mtvec, t0
        call main
        j .
    
stack_top :