#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define HEAP_START 0x01000000
#define HEAP_END   0x02000000
#define HEAP_ALIGNMENT 4

typedef struct heap_block{
    size_t size;
    bool free;
    struct heap_block* next;
}heap_block;

void* kmalloc(size_t size);
void kfree(void* address);

#endif
