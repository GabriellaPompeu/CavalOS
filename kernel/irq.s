.global irq0
.global irq1

.extern irq_handler

irq0:
    pushl $0          # err_code
    pushl $32         # int_no

    pusha

    pushl %ds

    pushl %esp
    call irq_handler
    addl $4, %esp

    popl %ds
    popa

    addl $8, %esp

    iret


irq1:
    pushl $0          # err_code
    pushl $33         # int_no

    pusha

    pushl %ds

    pushl %esp
    call irq_handler
    addl $4, %esp

    popl %ds
    popa

    addl $8, %esp

    iret