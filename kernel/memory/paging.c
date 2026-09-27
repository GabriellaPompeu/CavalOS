#include "paging.h"
#include "pmm.h"

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
}
