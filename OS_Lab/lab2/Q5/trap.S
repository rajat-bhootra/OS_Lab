    .section .text
    .global trap_handler
    .align 2
/*
   trap_handler:
     - Reads mcause.
     - If mcause indicates a machine external interrupt (code 11 with MSB=1),
       branch to external_isr.
     - Otherwise, just return.
*/
trap_handler:
    csrr t0, mcause         # t0 = mcause
    li   t1, 0x8000000B     # 0x8000000B: machine external interrupt (interrupt flag set, code 11)
    beq  t0, t1, external_isr
    mret

external_isr:
    # Read the claimed interrupt from the PLIC claim register (hart 0).
    li   t0, 0x0C000004     # PLIC_CLAIM address for hart 0
    lw   t1, 0(t0)          # t1 = claimed interrupt ID
    li   t2, 10             # KEYBOARD_IRQ_ID is 10
    bne  t1, t2, complete_ext  # If not keyboard, skip our handler
    call keyboard_isr       # Call keyboard ISR (in keyboard.c)
complete_ext:
    # Write the claimed ID back to PLIC_CLAIM to complete the interrupt.
    sw   t1, 0(t0)
    mret
