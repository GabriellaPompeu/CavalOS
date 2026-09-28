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
    uint32_t directory_index = virtual_address >> 22;
    uint32_t table_index = (virtual_address >> 12) & 0x3FF;

    if(directory_index >= 1024) return;

    if(kernel_directory->entries[directory_index].present == 0){
        page_table* table = create_page_table();
        if(table == NULL) return;
        uint32_t table_address = (uint32_t)table;

        kernel_directory->entries[directory_index].present = 1;
        kernel_directory->entries[directory_index].read_writer = 1;
        kernel_directory->entries[directory_index].frame =table_address >> 12;
    }

    page_table* table = (page_table*)(kernel_directory->entries[directory_index].frame << 12);

    table->entries[table_index].present = 1;
    table->entries[table_index].read_writer = 1;
    table->entries[table_index].frame = physical_address >> 12;
}

void unmap_page(uint32_t virtual_address){
    page_entry* page = get_page(virtual_address);

    if(page == NULL) return;

    page -> present = 0;
    page -> read_writer = 0;
    page -> user = 0;
    page -> unused = 0;
    page -> frame = 0;
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

void add_page_table(uint32_t directory_index, page_table* table){
    uint32_t table_address = (uint32_t)table;

    kernel_directory -> entries[directory_index].present = 1;
    kernel_directory -> entries[directory_index].read_writer = 1;
    kernel_directory -> entries[directory_index].frame = table_address >> 12;
}

page_entry* get_page(uint32_t virtual_address){
    uint32_t directory_index = virtual_address >> 22;
    uint32_t table_index = (virtual_address >> 12) & 0x3FF;

    if(directory_index >= 1024) return NULL;
    if(kernel_directory -> entries[directory_index].present == 0) return NULL;

    page_table* table = (page_table*)(kernel_directory -> entries[directory_index].frame << 12);
    return &table -> entries[table_index];
}
