    .section .text
    .global trap_handler
    .align 2
trap_handler:
    csrr t0, mcause         # Read mcause into t0
    li   t1, 0x80000007     # Constant for machine timer interrupt (bit31=1, code=7)
    beq  t0, t1, timer_isr  # If mcause equals timer interrupt, branch
    mret                    # Otherwise, return from trap
timer_isr:
    call timer_interrupt    # Call our C timer_interrupt() function
    mret                    # Return from trap
