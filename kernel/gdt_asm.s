.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    lgdt gp

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    ljmp $0x08, $1f
1:    
    ret

.size gdt_flush, . - gdt_flush

