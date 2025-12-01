#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#define kfree(pointer) do {  \
               \
    free(pointer);            \
    pointer = (void*)0;\
               \
               \
               \
               \
               \
} while (0)
#define HEAP_SIZE 1024 * 1024
#define MAGIC_FIRST 99999
#define MAGIC_MIDDLE 88888
#define MAGIC_LAST 777777
#define MAGIC_GONE 555555


// We will construct the heap like a linked list.
typedef struct heap {
    uint32_t size; // How big is this memory block.
    uint32_t flag; // Is memory free or not?
    uint32_t prev_size; // The previous blocks size
    uint32_t magic;

} __attribute__((packed));

void heap_init();
void* kmalloc(uint32_t size);
void free(void* pointer);
void test_kmalloc_kfree();
void print_heap();


#endif