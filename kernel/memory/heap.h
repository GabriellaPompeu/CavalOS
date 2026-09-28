#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>

#define HEAP_START 0x01000000
#define HEAP_END   0x02000000

void* kmalloc(size_t size);
void kfree(void* address);

#endif
