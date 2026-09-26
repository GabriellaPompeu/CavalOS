#include <stdint.h>
#include "interrupts.h"

extern void terminal_printf(const char* format, ...);

uint32_t __stack_chk_guard = 0xDEADBEEF;

void __stack_chk_fail(void){
    terminal_printf("KERNEL: STACK SMASHING DETECTADO\n");
    kernel_panic();

}
