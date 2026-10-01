#include "irq.h"
#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21

#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define PIC_EOI      0x20

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01

#define ICW4_8086    0x01

#define PIC1_OFFSET  0x20
#define PIC2_OFFSET  0x28

void outb(uint16_t port, uint8_t value)
{
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "Nd"(port)
    );
}

void irq_init(void)
{
    /** Começa a inicialização do PIC 1 e PIC 2.*/
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    /** Define os offsets das IRQs na IDT.
     *
     * PIC 1:
     * IRQ0 -> vetor 32
     * IRQ1 -> vetor 33
     * ...
     *
     * PIC 2:
     * IRQ8  -> vetor 40
     * IRQ9  -> vetor 41
     * ...*/
    outb(PIC1_DATA, PIC1_OFFSET);
    outb(PIC2_DATA, PIC2_OFFSET);

    /** Diz ao PIC 1 que existe um PIC 2 ligado na IRQ2. */
    outb(PIC1_DATA, 4);

    /** Diz ao PIC 2 que ele está ligado na IRQ2 do PIC 1.*/
    outb(PIC2_DATA, 2);

    /** Define o modo 8086.*/
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    /** Por enquanto, mantém todas as IRQs mascaradas.
     *
     * Depois vamos liberar:
     * IRQ0 -> timer
     * IRQ1 -> teclado*/
    outb(PIC1_DATA, 0xFF);
    outb(PIC2_DATA, 0xFF);
}

void irq_enable(uint8_t irq)
{
    if (irq < 8)
    {
        uint8_t mask;

        __asm__ volatile (
            "inb %1, %0"
            : "=a"(mask)
            : "Nd"((uint16_t)0x21)
        );

        mask &= ~(1 << irq);

        outb(0x21, mask);
    }
    else if (irq < 16)
    {
        uint8_t mask;

        __asm__ volatile (
            "inb %1, %0"
            : "=a"(mask)
            : "Nd"((uint16_t)0xA1)
        );

        mask &= ~(1 << (irq - 8));

        outb(0xA1, mask);
    }
}