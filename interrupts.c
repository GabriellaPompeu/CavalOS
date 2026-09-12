#include <stdbool.h>
#include <stddef.h>
#include <stdint.h> 
#include "interrupts.h"

//#define IDT_ENTRIES 256

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void idt_set_gate(uint8_t index, uint32_t handler, uint16_t selector, uint8_t type_attributes){
    idt[index].offset_low = handler & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attributes = type_attributes;
    idt[index].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init(void){
    for (size_t i = 0; i < IDT_ENTRIES; i++){
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attributes = 0;
        idt[i].offset_high = 0;
    }

    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;
}
