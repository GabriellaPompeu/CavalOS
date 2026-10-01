#include "pmm.h"

#define MAX_MEMORY 128 * 1024 * 1024
#define MAX_FRAMES (MAX_MEMORY / FRAME_SIZE)
#define PMM_NO_FRAME 0xFFFFFFFF

static uint8_t bitmap[MAX_FRAMES / 8];
static uint32_t total_frames;

static void set_frame(uint32_t frame){
    bitmap[frame / 8] |= (1 << (frame % 8));
}

static void clear_frame(uint32_t frame){
    bitmap[frame / 8] &= ~(1 << (frame % 8));
}

static int test_frame(uint32_t frame){
    return bitmap[frame / 8] & (1 << (frame % 8));
}

static void reserve_all_frames(void){
    for(uint32_t i = 0; i < sizeof(bitmap); i++) bitmap[i] = 0xFF;
}

void pmm_init(uint32_t memory_size){
    if(memory_size > MAX_MEMORY) memory_size = MAX_MEMORY;
    total_frames = memory_size / FRAME_SIZE;
    reserve_all_frames();
}

void pmm_free_region(uint32_t address, uint32_t length){
    uint32_t start_frame = (address + FRAME_SIZE - 1) / FRAME_SIZE;
    uint32_t end_frame = (address + length) / FRAME_SIZE;

    for(uint32_t frame = start_frame; frame < end_frame; frame++){
        if(frame >= total_frames) break;
        clear_frame(frame);
    }
}

void pmm_reserve_region(uint32_t address, uint32_t lenght){
    uint32_t start_frame = address / FRAME_SIZE;
    uint32_t end_frame = (address + lenght + FRAME_SIZE - 1) / FRAME_SIZE;

    for(uint32_t frame = start_frame; frame < end_frame; frame++){
        if(frame >= total_frames) break;
        set_frame(frame);
    }
}

uint32_t pmm_get_bitmap_address(void){
    return (uint32_t)bitmap;
}

uint32_t pmm_get_bitmap_size(void){
    return sizeof(bitmap);
}

uint32_t alloc_frame(void){
    for(uint32_t frame = 0; frame < total_frames; frame++){
        if(!test_frame(frame)){
            set_frame(frame);
            return frame * FRAME_SIZE;
        }
    }
    return PMM_NO_FRAME;
}

void free_frame(uint32_t address){
    uint32_t frame = address / FRAME_SIZE;
    if(frame >= total_frames) return;
    clear_frame(frame);
}

uint32_t pmm_get_total_frames(void)
{
    return total_frames;
}

uint32_t pmm_get_used_frames(void){
    uint32_t used_frames = 0;

    for (uint32_t frame = 0; frame < total_frames; frame++){
        if (test_frame(frame)) used_frames++;
    }

    return used_frames;
}

uint32_t pmm_get_free_frames(void){
    return total_frames - pmm_get_used_frames();
}
