.global trap_handler
.extern trp
trap_handler:
    call trp
    mret