#include <stdbool.h>
#include <stddef.h>
#include <stdint.h> 
#include "interrupts.h"

#define IDT_ENTRIES 256

struct idt_entry idt[IDT_ENTRIES];

void idt_init(){
    for (size_t i = 0; i < IDT_ENTRIES; i++){
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attributes = 0;
        idt[i].offset_high = 0;
    }
}