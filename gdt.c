#include <stdint.h>
#include "gdt.h"

struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gp;
struct tss_entry tss;

void gdt_set_entry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity){
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_middle = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;

    gdt[index].access = access;

    gdt[index].granularity = (limit >> 16) & 0x0F;
    gdt[index].granularity |= granularity & 0xF0;
}

static void tss_init(void){
    uint8_t* tss_bytes = (uint8_t*)&tss;

    for(uint32_t i = 0; i < sizeof(tss); i++) tss_bytes[i] = 0;

    tss.esp0 = 0;
    tss.ss0 = 0x10;
    tss.iomap_base = sizeof(tss);

    gdt_set_entry(5, (uint32_t)&tss, sizeof(tss) - 1, 0x89, 0x00);
}

void gdt_init(void){
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    gdt_set_entry(0, 0, 0, 0, 0);
    gdt_set_entry(1, 0, 0xFFFFF, 0x9A, 0xCF);
    gdt_set_entry(2, 0, 0xFFFFF, 0x92, 0xCF);
    gdt_set_entry(3, 0, 0xFFFFF, 0xFA, 0xCF);
    gdt_set_entry(4, 0, 0xFFFFF, 0xF2, 0xCF);

    tss_init();
}
