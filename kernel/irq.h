#ifndef IRQ_H
#define IRQ_H
#include "interrupts.h"
#include <stdint.h>

void irq_init(void);
void irq_enable(uint8_t irq);
void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
void reboot(void);

void irq_handler(uint32_t int_no);

#endif
