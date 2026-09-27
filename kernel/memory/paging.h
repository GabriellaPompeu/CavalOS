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

#endif
