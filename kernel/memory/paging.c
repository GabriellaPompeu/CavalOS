#include "paging.h"
#include "pmm.h"
#include <stddef.h>

static page_directory* kernel_directory;

void pagina_init(void){
    uint32_t directory_frame = alloc_frame();

    if(directory_frame == 0xFFFFFFFF) return;

    kernel_directory = (page_directory*)directory_frame;

    for(uint32_t i = 0; i < 1024; i++){
        kernel_directory -> entries[i].present = 0;
        kernel_directory -> entries[i].read_writer = 0;
        kernel_directory -> entries[i].user = 0;
        kernel_directory -> entries[i].unused = 0;
        kernel_directory -> entries[i].frame = 0;
    }

    uint32_t table_frame = alloc_frame();

    if(table_frame == 0xFFFFFFFF) return;

    page_table* first_table = (page_table*)table_frame;

    for(uint32_t i = 0; i < 1024; i++){
        first_table -> entries[i].present = 0;
        first_table -> entries[i].read_writer = 0;
        first_table -> entries[i].user = 0;
        first_table -> entries[i].unused = 0;
        first_table -> entries[i].frame = 0;
    }

    kernel_directory -> entries[0].present = 1;
    kernel_directory -> entries[0].read_writer = 1;
    kernel_directory -> entries[0].frame = table_frame >> 12;

    for(uint32_t i = 0; i < 1024; i++){
        uint32_t frame = alloc_frame;

        if(frame == 0xFFFFFFFF) return;

        first_table -> entries[i].present = 1;
        first_table -> entries[i].read_writer = 1;
        first_table -> entries[i].frame = frame >> 12;
    }
}

void map_page(uint32_t virtual_address, uint32_t physical_address){
    uint32_t page_index = virtual_address >> 12;

    if(page_index >= 1024) return;

    page_table* first_table = (page_table*)(kernel_directory -> entries[0].frame << 12);

    first_table -> entries[page_index].present = 1;
    first_table -> entries[page_index].read_writer = 1;
    first_table -> entries[page_index].frame = physical_address >> 12;
}

page_table* create_page_table(void){
    uint32_t table_frame = alloc_frame();

    if(table_frame == 0xFFFFFFFF) return NULL;

    page_table* table = (page_table*)table_frame;

    for(uint32_t i = 0; i < 1024; i++){
        table -> entries[i].present = 0;
        table -> entries[i].read_writer = 0;
        table -> entries[i].user = 0;
        table -> entries[i].unused = 0;
        table -> entries[i].frame = 0;
    }
    return table;
}
