    .section .text
    .global trap_handler
    .extern trap_handler_c
trap_handler:
    # save_registers
    call trap_handler_c    # Call the C trap handler
    # restore_registers
    mret                   # Return from trap
