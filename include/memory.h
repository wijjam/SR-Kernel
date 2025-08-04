#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>


// We will construct the heap like a linked list.
typedef struct heap {
    uint32_t size; // How big is this memory block.
    uint8_t flag; // Is memory free or not?
    uint32_t prev_size; // The previous blocks size
    uint8_t magic;
};

void heap_init();
void* kmalloc(uint16_t size);
void kfree(void* pointer);
void debug_heap();


#endif