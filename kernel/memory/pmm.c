#include "pmm.h"

#define MAX_MEMORY 128 * 1024 * 1024

#define MAX_FRAMES (MAX_MEMORY / FRAME_SIZE)

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

    for(uint32_t i = 0; i < sizeof(bitmap); i++){

        bitmap[i] = 0xFF;

    }

}

void pmm_init(uint32_t memory_size){

    total_frames = memory_size / FRAME_SIZE;

    reserve_all_frames();

}

void pmm_free_region(uint32_t address, uint32_t length){

    uint32_t start_frame = address / FRAME_SIZE;
    uint32_t frame_count = length / FRAME_SIZE;

    for(uint32_t i = 0; i < frame_count; i++){

        clear_frame(start_frame + i);

    }

}

uint32_t alloc_frame(void){

    for(uint32_t frame = 0; frame < total_frames; frame++){

        if(!test_frame(frame)){

            set_frame(frame);

            return frame * FRAME_SIZE;

        }

    }

    return 0;

}

void free_frame(uint32_t address){

    uint32_t frame = address / FRAME_SIZE;

    clear_frame(frame);

}
