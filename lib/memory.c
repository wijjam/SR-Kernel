#include "../include/memory.h"
#include "../include/vga.h"


extern _end[];
// This below finds the end of kernel memory address
volatile struct heap* start_heap_memory = (volatile struct heap*) _end;
// heap_end is now 16-bits for the header memory and 8-bits for data memory from the end
struct heap* heap_end = ((struct heap*)((char*)((volatile struct heap*) _end) + HEAP_SIZE - sizeof(struct heap) - 8));

void heap_init() {
    
    start_heap_memory->flag = 0;
    start_heap_memory->size = HEAP_SIZE; // Here we give the heap 1MB, which includes all the headers
    start_heap_memory->prev_size = 0;
    start_heap_memory->magic = MAGIC_FIRST;
    kprintf("Initial heap size: %d\n", start_heap_memory->size);
    kprintf("The sizeof heap: %d\n", sizeof(struct heap));
    kprintf("The end of the heap total: %d\n", (char*)start_heap_memory + HEAP_SIZE);
    kprintf("The end of the heap with header and memory space: %d\n", heap_end);
}

void* kmalloc(uint32_t size) {
    struct heap* current_block = start_heap_memory;
    //full_size_request is the real size the user is asking for. So they size they want for memory and then the hidden head.
    uint32_t full_size_request = size + sizeof(struct heap);

    if (full_size_request > HEAP_SIZE-sizeof(struct heap)) {
        kprintf("%eRequested size exceeds available heap size.\n");
        return (void*)0;  // Too big for heap
    }

    if (size <= 0) { // This stays as just a size check since if the user writes 0 we will stop it and not assume.
        kprintf("%eExcuse me but you can't do anything with a memory with 0 bits. Good try but no..\n");
        kprintf("%eProblem encountered in the malloc size <= 0 check\n");
        return (void*)0;
    }
   
    while (current_block < heap_end) {



            uint32_t data_size = current_block->size - sizeof(struct heap);
        
        // If the current block is free we pass into this check.
        // We do not check for magic last because heap_end should not be able to get down here. But extra percasun
        if (current_block->flag == 0 && (current_block->magic == MAGIC_FIRST || current_block->magic == MAGIC_MIDDLE)) {
            // if the free block has memory enough for the requested size then we pass into this check.
            if (data_size >= full_size_request) {

                 uint32_t old_full_size = current_block->size;
                 //kprintf("The old full size when initialized: %d\n", old_full_size);
                // We check if the first block is the one we are checking. if it is we do not change the magic.
                if (current_block->prev_size == 0) {
                    
                    // The old full size is what will save and the be used to get the new size for the new block being split.
                    current_block->flag = 1;
                    current_block->prev_size = 0;
                    current_block->size = full_size_request;
                    current_block->magic = MAGIC_FIRST;
                } else {
                    // Implement logic to continue spliting and changing when not at the begining later here.
                    current_block->flag = 1;
                    current_block->size = full_size_request;
                }
                // With this we move forward with just the requested size.
                current_block = (struct heap*)((char*)current_block + full_size_request);
                // Set middle here since this will always be after the first block and therefore always middle or last.
                // We check last below so this magic is safe and if we are on the last block then we change it to last.
                // We asume middle.
                current_block->magic = MAGIC_MIDDLE;
                current_block->flag = 0;
                current_block->prev_size = full_size_request;
                current_block->size = old_full_size - full_size_request;

               
                return ((char*)current_block - current_block->prev_size) + sizeof(struct heap);
            }

        }

        //kprintf("We are stuck WEEEEEEEE");

        // No check needed since the only place a next does not exist is on the heap_end
        // Since the while stops if we are on it, we wont have to make a check here for it.
        // Therefore we only go forward once and check again to find flag = 1;
        //kprintf("The size of the blocks being passed: %d, \n", current_block->size);
        current_block = (struct heap*)((char*)current_block + current_block->size);


    }


    return (void*)0;
}

void free(void* pointer) {

    // Checks the inputed pointer for NILL, exeding heap memory
    if (pointer == (void*)0 || pointer < (void*)start_heap_memory || pointer > (void*)heap_end) {
        kprintf("\n%e Sorry but you are trying to free a block that is out of bounds, or just trying to free a null 1\n");
        return;
    }
    // Checking if the current header gotten from the pointer is in the heap memory
    struct heap* current_block = (struct heap*)((char*)pointer - sizeof(struct heap));
    if (current_block < start_heap_memory) {
        kprintf("\n%e Sorry but you are trying to free a block that is out of bounds 2\n");
        return;
    }
    if (current_block->size == 0) {
        kprintf("\n%e Error: Block size is 0, indicating corruption.\n");
        return;
    }
    // check if the current pointer is a merged and unusable header, if it is then print error and return. !
    //kprintf("Current_block magic: %d\n", current_block->magic);
    if (!(current_block->magic == MAGIC_FIRST || current_block->magic == MAGIC_MIDDLE)) {
        kprintf("\n%eEhm sorry but... freeing a invlid header is not really allowed so...... yeah\n");
        kprintf("\n%eTried to free a corrupted pointer in kfree()\n");
        return;
    }
    if (current_block->flag == 0) {
        kprintf("\n%e Warning: Double-free attempt detected.\n");
        return;
    }

    // check if the size in current head is overflowing, if it is then print a error and return.
    if (current_block->size > HEAP_SIZE) {
        kprintf("Corrupted size in heap block exeding heap end");
        return;
    }

    current_block->flag = 0;
    
    // We make sure that we only go backwards if we are not on the first block, because backwards from there is outside of the heap.
    if (current_block->magic != MAGIC_FIRST) {
        struct heap* prev_block = (struct heap*)((char*)current_block - current_block->prev_size);
        // Stop once we reach the left most block
        //kprintf("\n=========Just before the first loop =============\n");

        while (prev_block->flag == 0) {
            

            // If we are on the first one we stop
            if (current_block->magic == MAGIC_FIRST) {
                break;
                // We just continue if we are on the middle.
            } else if (current_block->magic == MAGIC_MIDDLE) {

            } else {
                // If we reach this then there is a error since the block is useless.
                kprintf("The magic is: %d\n", current_block->magic);
                kprintf("%eBlock chain error detected. in the back loop\n");
                return;
            }

            current_block = prev_block;
            prev_block = (struct heap*)((char*)current_block - current_block->prev_size);

        }
    }



    // Check if the next header block to the right has a valid address inside the heap and if it's previous variable is valid.
    struct heap* next_block = ((struct heap*)((char*)current_block + (current_block->size)));
    // Save the current left most block in start_block.
    struct heap* start_block = current_block;
    if ((char*)next_block - next_block->prev_size != (char*)current_block) {
        kprintf("%eUGH next blocks previous is not current. error..\n");
        return;
    } 
    while (next_block < heap_end) { 


        if (next_block->flag == 0 && next_block->magic != MAGIC_GONE) {

            start_block->size += next_block->size;
            next_block->magic = MAGIC_GONE;
            ((struct heap*)((char*)next_block + (next_block->size)))->prev_size = start_block->size;

            if (start_block->size > HEAP_SIZE) {
                kprintf("%e This is illegal!!!\n");
                kprintf("For some reason in the second loop the size of start_block is: %d\n", start_block->size);
            }

            current_block = next_block;
            next_block = ((struct heap*)((char*)current_block + (current_block->size)));

        } else {
            break;
        }    

    }









}






void test_kmalloc_kfree() {

    char* ptr = kmalloc(200);
    char* ptr2 = kmalloc(200);


    if (ptr == (void*)0) {
        kprintf("%eHEy no null pointers in my house.\n");
        return;
    }

    ptr[0] = 'C';
    ptr[1] = 'A';
    ptr[2] = 'K';
    ptr[3] = 'E';
    ptr[4] = '\0';

    kprintf("Current word: %s\n", ptr);
    print_heap();

    kfree(ptr);
    print_heap();
    
    
    

}

void print_heap() {
    struct heap* current = start_heap_memory;
    struct heap* next = (struct heap*)((char*)current + current->size);
    while (current < heap_end) {

        kprintf("We are at address: [%d] with state: [%d] and the size is: [%d] \n", current, current->flag, current->size);
        current = next;
        next = (struct heap*)((char*)current + current->size);
        
    }
    kprintf("\n");
}



