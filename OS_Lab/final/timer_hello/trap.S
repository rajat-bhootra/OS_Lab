.global trap_handler
.extern trap
trap_handler:
    call trap
    mret