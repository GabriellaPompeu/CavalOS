#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MULTIBOOT_MEMORY_AVAILABLE 1

typedef struct{
    uint32_t size;

    uint32_t addr_low; uint32_t addr_high;

    uint32_t len_low; uint32_t len_high;

    uint32_t type;
} multiboot_memory_map_entry;

void memory_map_init(uint32_t multiboot_info_addr);

#endif
