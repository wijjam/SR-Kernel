#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>


// We will construct the heap like a linked list.
typedef struct heap {
    uint16_t size; // How big is this memory block.
    uint8_t flag; // Is memory free or not?
    uint16_t prev_size; // The previous blocks size
};

void heap_init();
void* kmalloc(uint16_t size);
void free(void* pointer);


#endif