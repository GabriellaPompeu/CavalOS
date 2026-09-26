#include "memory.h"
#include "pmm.h"

extern void terminal_printf(const char* format, ...);

typedef struct{
    uint32_t flags;

    uint32_t mem_lower; uint32_t mem_upper;

    uint32_t boot_device; uint32_t cmdline;

    uint32_t mods_count; uint32_t mods_addr;

    uint32_t syms[4];

    uint32_t mmap_lenght; uint32_t mmap_addr;
} multiboot_info;

void memory_map_init(uint32_t multiboot_info_addr){

    multiboot_info* info = (multiboot_info*)multiboot_info_addr;

    terminal_printf("Memory Map:\n");

    uint32_t current = info->mmap_addr;

    uint32_t end = info->mmap_addr + info->mmap_lenght;

    uint32_t max_address = 0;

    /*
     * Primeira passagem:
     * descobrir o maior endereço do Memory Map.
     */

    while(current < end){

        multiboot_memory_map_entry* entry = (multiboot_memory_map_entry*)current;

        uint32_t region_end = entry->addr_low + entry->len_low;

        if(region_end > max_address) max_address = region_end;

        current += entry->size + sizeof(entry->size);
    }

    /*
     * Inicializa o PMM.
     */

    pmm_init(max_address);

    /*
     * Segunda passagem:
     * liberar as regiões de memória disponíveis.
     */

    current = info->mmap_addr;

    while(current < end){

        multiboot_memory_map_entry* entry = (multiboot_memory_map_entry*)current;

        terminal_printf("Endereço: %d | Tamanho: %d | Tipo: %d\n", entry->addr_low, entry->len_low, entry->type);

        if(entry->type == MULTIBOOT_MEMORY_AVAILABLE) pmm_free_region(entry->addr_low, entry->len_low);

        current += entry->size + sizeof(entry->size);
    }
}
