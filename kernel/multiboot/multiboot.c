#include <stdint.h>

#include "multiboot.h"

extern void terminal_printf(const char* format, ...);
extern void terminal_write_string(const char* data);

int multiboot_validate(uint32_t magic){
    return magic == MULTIBOOT_BOOTLOADER_MAGIC;
}

int multiboot_has_memory_map(const struct multiboot_info* info){
    if (info == 0) return 0;
    
    return (info->flags & MULTIBOOT_INFO_MEM_MAP) != 0;
}

void multiboot_print_memory_map(const struct multiboot_info* info){
    if (info == 0) return;

    if (!multiboot_has_memory_map(info)) {
    terminal_write_string("Multiboot: mapa de memoria nao disponivel.\n");

    return;
    }

    terminal_write_string("\n=== MAPA DE MEMORIA ===\n");

    uint32_t current = info->mmap_addr;
    uint32_t end = info->mmap_addr + info->mmap_length;

    while (current < end) {
        struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*)current;

        terminal_printf(
            "Base: %d | Tamanho: %d | Tipo: %d\n",
            (int)entry->base_addr,
            (int)entry->length,
            (int)entry->type
        );

        current += entry->size + sizeof(entry->size);
    }

    terminal_write_string("=== FIM DO MAPA ===\n");
}
