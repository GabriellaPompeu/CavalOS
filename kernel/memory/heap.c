#include "heap.h"

static uint32_t heap_current = HEAP_START;

void* kmalloc(size_t size){
    if(size == 0) return NULL;

    if(heap_current + size > HEAP_END) return NULL;

    void* address = (void*)heap_current;
    heap_current += size;
    return address;
}

void kfree(void* address){
    (void)address;
}
