#include "../include/memory.h"
#include "../include/vga.h"


extern _end[];
// This below finds the end of kernel memory address
volatile struct heap* start_heap_memory = (volatile struct heap*) _end;

void heap_init() {
    
    start_heap_memory->flag = 1;
    start_heap_memory->size = (1024 * 1024) - sizeof(struct heap); // Here we give the heap 1MB
                                                                   // We then remove the size of heap head from the total memory and start from there.
    start_heap_memory->prev_size = 0;
    
}


void* kmalloc(uint16_t size) {

    struct heap* heap_end = (struct heap*)((char*)start_heap_memory + 1024 * 1024);
    struct heap* current_block = start_heap_memory;

    // Loops untill we get to the end of the heap
    while (current_block < heap_end) {

        if (current_block->size >= size && current_block->flag == 1) {
            // We save the current block we are at
            struct heap* allocated_block = current_block;
            // we save the original size
            int old_size = current_block->size;
            current_block->size = size;
            current_block->flag = 0;
            // we move on to the next block and split if it is worth it. So if the differece is more than 5 bytes
            current_block = (struct heap*)((char*)current_block + current_block->size);
            if (old_size - size < 5) {
                return (void*)((char*)allocated_block + sizeof(struct heap));
            } else {
                current_block->size = old_size - size;
                current_block->flag = 1;
                current_block->prev_size = size;
                return (void*)((char*)allocated_block + sizeof(struct heap));
            }
        }

        current_block = (struct heap*)((char*)current_block + current_block->size);
    }
    return (void*)0;
}

void free(void* pointer) {

    struct heap* heap_end = (struct heap*)((char*)start_heap_memory + 1024 * 1024);
    struct heap* current_block = (struct heap*)((char*)pointer - sizeof(struct heap));
    
    if (pointer == (void*)0) {
        kprintf("%eEhm not sure why you want to free NULL, fix please\n");
        return;
    }

    if (current_block > start_heap_memory) {
        return;
    }

    current_block->flag = 1;
    while (1 == 1) {
        if (current_block > start_heap_memory) {
            if ((struct heap*)((char*)current_block - (current_block->prev_size + sizeof(struct heap))) >= start_heap_memory) {
                if (((struct heap*)((char*)current_block - (current_block->prev_size + sizeof(struct heap))))->flag == 1) {
                    current_block = (struct heap*)((char*)current_block - (current_block->prev_size + sizeof(struct heap)));
                } else {
                    break;
                }
            }
        } else if (current_block->prev_size == 0) {
            break;
        } else {
            break;
           }
    }

    uint32_t size = current_block->size;

    while (current_block < heap_end) {
        if ((struct heap*)((char*)current_block + (current_block->size + sizeof(struct heap))) <= heap_end) {

        }
    }

}



