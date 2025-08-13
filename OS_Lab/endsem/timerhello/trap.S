.global trap_handler
.extern print

trap_handler :
    call print
    mret