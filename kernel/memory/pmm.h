#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define FRAME_SIZE 4096

void pmm_init(uint32_t memory_size);

void pmm_free_region(uint32_t address, uint32_t length);

void pmm_reserve_region(uint32_t address, uint32_t lenght);

uint32_t pmm_get_bitmap_address(void);

uint32_t pmm_get_bitmap_size(void);

uint32_t alloc_frame(void);

void free_frame(uint32_t address);

#endif
