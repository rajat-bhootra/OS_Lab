.global _start
.extern trap_handler
.extern main

_start :
        la sp ,stack_top
        la t0,trap_handler
        csrw mtvec,t0
        call main
        j .

stack_top : 