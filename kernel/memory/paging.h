#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#define PAGE_SIZE 4096

typedef struct{
    uint32_t present     :1;
    uint32_t read_writer :1;
    uint32_t user        :1;
    uint32_t unused      :9;
    uint32_t frame       :20;
}page_entry;

typedef struct{
    page_entry entries[1024];
}page_table;

typedef struct{
    page_entry entries[1024];
}page_directory;

void paging_init(void);

void map_page(uint32_t virtual_address, uint32_t physical_address);

page_table* create_page_table(void);

void add_page_table(uint32_t directory_index, page_table* table);

page_entry* get_page(uint32_t virtual_address);

void unmap_page(uint32_t virtual_address);

#endif
