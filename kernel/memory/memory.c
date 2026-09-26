#include "memory.h"
#include "pmm.h"

extern void terminal_printf(const char* format, ...);
extern uint32_t kernel_start;
extern uint32_t kernel_end;

typedef struct{
    uint32_t flags;

    uint32_t mem_lower; uint32_t mem_upper;

    uint32_t boot_device; uint32_t cmdline;

    uint32_t mods_count; uint32_t mods_addr;

    uint32_t syms[4];

    uint32_t mmap_lenght; uint32_t mmap_addr;
} multiboot_info;

static void pmm_test(void){
    uint32_t frame1 = alloc_frame();
    uint32_t frame2 = alloc_frame();

    terminal_printf("PMM TEST\n");
    terminal_printf("Frame 1: %x\n", frame1);
    terminal_printf("Frame 2: %x\n", frame2);

    free_frame(frame1);

    uint32_t frame3 = alloc_frame();

    terminal_printf("Frame 3: %x\n", frame3);
}

void memory_map_init(uint32_t multiboot_info_addr){

    multiboot_info* info = (multiboot_info*)multiboot_info_addr;

    uint32_t current = info->mmap_addr;

    uint32_t end = info->mmap_addr + info->mmap_lenght;

    uint32_t max_address = 0;
    /** Primeira passagem:
     * descobrir o maior endereço do Memory Map.*/
    while(current < end){
        multiboot_memory_map_entry* entry = (multiboot_memory_map_entry*)current;

        uint32_t region_end = entry->addr_low + entry->len_low;

        if(region_end > max_address) max_address = region_end;

        current += entry->size + sizeof(entry->size);
    }
    // Inicializa o PMM
    pmm_init(max_address);
    /** Segunda passagem:
     * liberar as regiões de memória disponíveis.*/
    current = info->mmap_addr;

    while(current < end){
        multiboot_memory_map_entry* entry = (multiboot_memory_map_entry*)current;

        terminal_printf("Endereco: %d | Tamanho: %d | Tipo: %d\n", entry->addr_low, entry->len_low, entry->type);

        if(entry->type == MULTIBOOT_MEMORY_AVAILABLE) pmm_free_region(entry->addr_low, entry->len_low);

        current += entry->size + sizeof(entry->size);
    }

    pmm_reserve_region((uint32_t)&kernel_start, (uint32_t)&kernel_end - (uint32_t)&kernel_start);
    pmm_reserve_region(pmm_get_bitmap_address(), pmm_get_bitmap_size());
    pmm_test();
}
