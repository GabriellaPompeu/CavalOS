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

struct idt_ptr{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));

void idt_init(void);

void idt_set_gate(uint8_t index, uint32_t handler, uint16_t selector, uint8_t type_attributes);

#endif
