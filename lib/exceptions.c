#include "../include/exceptions.h"
#include "../include/vga.h"

void divide_by_zero_handler() {

    kprintf("%e Divide by zero exception occurred!\n");
    kprintf("%e ehm..... no? That is not allowed in math or in the kernel. Like do you know how uppset the cpu is? You have him constantly trying to solve this equation.\n");
    kprintf("%e a/b = c => cb = a => 0 = cb - a (a can not be 0 since 0/0 is a whole other torture.) When in 0 = cb - a where a can't be 0 can it become true?");
    kprintf("%e Never think about it doing 0 = cb - a would be infinite and the cpu would be stuck in a loop.\n");
    kprintf("%e and you just like... divide by zero? That is not nice.\n");
    while (1) {} // Halt the system or perform other error handling
}

void page_fault_handler() {

    kprintf("%e Page fault exception occurred!\n");
    while (1) {} // Halt the system or perform other error handling
}

void general_protection_fault_handler(uint32_t error_code) {

    kprintf("%e General protection fault exception occurred!\n");

    if (error_code == 0) {
        kprintf("-> %eBad instruction or memory access\n");
        kprintf("-> %eCheck: Stack pointer, EIP, memory addresses\n");
        kprintf("-> %ePro tip: Pointers should point to ACTUAL mamoery (Chocking I know)\n");
        kprintf("-> %e So can you like not do that? Thanks.\n");
        } else if (error_code >= 8) {
        int segment_number = error_code / 8;
        kprintf("-> Problem with segment %d\n", segment_number);
        kprintf("-> %eCheck: CS, DS, SS registers\n");
        kprintf("-> %eCheck: Segment limits and permissions\n");
        kprintf("-> %e What have I told you about assuming segments? Stop it get some help.\n");
    } else {
        kprintf("-> %eWeird error code, good luck!\n");
        kprintf("-> %e Congrats you found a legendary error, but like this is not pokemon... good luck debging you need it.\n");
    }
    kprintf("\nHalting the system...\n");
    while (1) {} // Halt the system or perform other error handling
}

