#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define HEAP_SIZE 1024 * 1024
#define MAGIC_FIRST 99999
#define MAGIC_MIDDLE 88888
#define MAGIC_LAST 777777


// We will construct the heap like a linked list.
typedef struct heap {
    uint32_t size; // How big is this memory block.
    uint32_t flag; // Is memory free or not?
    uint32_t prev_size; // The previous blocks size
    uint32_t magic;

};

void heap_init();
void* kmalloc(uint32_t size);
void kfree(void* pointer);
void test_kmalloc_kfree();

#endif