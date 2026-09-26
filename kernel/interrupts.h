#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

#define IDT_ENTRIES 256

struct idt_entry{
    uint16_t offset_low; //parte baixa do end da função q trata a inter.
    uint16_t selector; //diz qual segmento do codigo deve ser utilizado quando o handler for executado.
    uint8_t zero; //é literalmente um 0
    uint8_t type_attributes; //informa as características do Gate.
    uint16_t offset_high; //parte superior do end da função q trata a inter. 
}__attribute__((packed));

struct idt_ptr{ //define o endereço que começa a idt e o maior byte que pode acessar
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

struct registers{
    uint32_t ds;

    uint32_t edi; uint32_t esi;
    uint32_t ebp; uint32_t esp;
    uint32_t ebx; uint32_t edx;
    uint32_t ecx; uint32_t eax;

    uint32_t int_no; uint32_t err_code;

    uint32_t eip; uint32_t cs;
    uint32_t eflags; uint32_t useresp;
    uint32_t ss;
};

void exception_handler(struct registers *r);

extern struct idt_entry idt[IDT_ENTRIES];
extern struct idt_ptr idtp;

void idt_init(void);

void idt_set_gate(uint8_t index, uint32_t handler, uint16_t selector, uint8_t type_attributes);

#endif
