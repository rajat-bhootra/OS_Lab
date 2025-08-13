.global _start
.extern main
_start:
    la sp, stack_top
    call main
    j .

stack_top: