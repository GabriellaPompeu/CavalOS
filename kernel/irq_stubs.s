.global irq0
.global irq1

.extern irq_handler

irq0:
    pusha

    pushl $32
    call irq_handler
    addl $4, %esp

    popa
    iret

irq1:
    pusha

    pushl $33
    call irq_handler
    addl $4, %esp

    popa
    iret