#include "heap.h"

static heap_block* first_block = NULL;

static size_t align_size(size_t size){
    return (size + HEAP_ALIGNMENT - 1) & ~(HEAP_ALIGNMENT - 1);
}

void* kmalloc(size_t size){
    if(size == 0) return NULL;

    size = align_size(size);

    heap_block* current = first_block;

    while(current != NULL){
        if(current->free && current->size >= size){

            /** Verifica se sobra espaço suficiente
             * para criar outro bloco.*/
            if(current->size >= size + sizeof(heap_block) + 1){
                heap_block* new_block = (heap_block*)((uint8_t*)(current + 1) + size);

                new_block->size = current->size - size - sizeof(heap_block);

                new_block->free = true;
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            current->free = false;
            return (void*)(current + 1);
        }
        current = current->next;
    }

    /** Não encontrou bloco livre.
     * Cria um novo bloco no final do heap.*/
    uint32_t block_address;

    if(first_block == NULL){
        block_address = HEAP_START;
    } else{
        heap_block* last = first_block;

        while(last->next != NULL) last = last->next;

        block_address = (uint32_t)(last + 1) + last->size;
    }

    if(block_address + sizeof(heap_block) + size > HEAP_END) return NULL;

    heap_block* new_block = (heap_block*)block_address;

    new_block->size = size;
    new_block->free = false;
    new_block->next = NULL;

    if(first_block == NULL) first_block = new_block;
    else{
        heap_block* last = first_block;

        while(last->next != NULL) last = last->next;

        last->next = new_block;
    }
    return (void*)(new_block + 1);
}

void kfree(void* address){
    if(address == NULL) return;

    heap_block* block = (heap_block*)address - 1;
    block -> free = true;

    heap_block* previous = NULL;
    heap_block* current = first_block;

    while(current != NULL && current != block){
        previous = current;
        current = current -> next;
    }

    if(block -> next != NULL && block -> next -> free){
        block -> size += sizeof(heap_block) + block -> next -> size;
        block -> next = block -> next -> next;
    }

    if(previous != NULL && previous -> free){
        previous -> size += sizeof(heap_block) + block -> size;
        previous -> next = block -> next;
    }
}
