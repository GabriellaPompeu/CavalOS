#include "timer.h"
#include "../irq.h"
#include "../process/process.h"

extern void terminal_printf(const char* format, ...);

#define PIT_COMMAND 0x43
#define PIT_CHANNEL0 0x40
#define PIT_FREQUENCY 1193182

static volatile uint32_t timer_ticks = 0;

void timer_init(uint32_t frequency){
    uint32_t divisor = PIT_FREQUENCY / frequency;
    outb(PIT_COMMAND, 0x36);

    outb(PIT_CHANNEL0, divisor & 0xFF);
    outb(PIT_CHANNEL0, (divisor >> 8) & 0xFF);
}

void timer_handler(void){ 
    timer_ticks++;
    if(timer_ticks % 10 == 0) scheduler_run_next();
}

uint32_t timer_get_ticks(void){
    return timer_ticks;
}
