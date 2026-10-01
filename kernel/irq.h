#ifndef IRQ_H
#define IRQ_H
#include "interrupts.h"
#include <stdint.h>

void irq_init(void);
void irq_enable(uint8_t irq);
void outb(uint16_t port, uint8_t value);

void irq_handler(uint32_t int_no);

#endif