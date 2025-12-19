#include <stdarg.h>
#include <stdint.h>
#include "include/io.h"
#include "include/rtc.h"
#include "include/vga.h"
#include "include/interrupts.h"
#include "include/pic.h"
#include "include/keyboard.h"
#include "include/commands.h"
#include "include/memory.h"
#include "include/process_manager.h"
#include "include/forgeproc.h"
#include "include/paging_manager.h"


void idle_process() {
       while(1) {
        // Do nothing, or print "IDLE" 
        // This keeps the CPU busy when all real processes sleep
        __asm__ volatile("hlt");
    }
}

void process_worker() {
    int process_pid = current_process->PID;
    sleep(200);

    fork();

    while(1) {

        
        //kprintf("process running right now: %d\n", process_pid);
        

        //sleep(100);
    
    }
}

void timer_process_worker() {

    while(1) {
        update_print_corner_time();
        sleep(1); // 100 is 1 second. 
    }
}

void worker_process() {

    
    while(1) {
        //kprintf("%d worker process running\n", current_process->PID);
        sleep(1000);

        if (current_process->PID == 2) {
            //kprintf("I am process 2 and I am happy YAY!\n");
        }

    }
}


void kernel_main(void) {

    pic_remap(32, 40);  // Remap IRQs: Master to 32-39, Slave to 40-47
    pic_disable_irq(0); // Disable timer for now (would overwhelm us)
    pic_enable_irq(1);  // Enable keyboard

    init_interrupts();
    heap_init();
    init_paging();
    init_keyboard();





    

    init_process_scheduler(&idle_process);

    create_process(&timer_process_worker);
    //create_process(&worker_process);

    __asm__ volatile ("sti"); // opens the flood gates.
    pic_enable_irq(0); // Enable timer
    

map_virtual_to_physical_address(0xDEAD0000, 0xB8000);

// Skriv via den nya adressen
uint16_t* virtual_video = (uint16_t*)0xDEAD0000;
virtual_video[0] = 0x0F00 | 'X';  // Vit 'X' i hörnet

kprintf("If you see an X in top-left, paging works!\n");

    // Main kernel loop - just wait for interrupts
    while (1) {
        // Do nothing - let interrupts handle everything
        // This is where your kernel "idles"
        __asm__ volatile("hlt");
    }



}

/*
void test_simple_jump() {
    uint16_t actual_cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(actual_cs));
    
    uint32_t current_eflags;
    __asm__ volatile("pushf; pop %0" : "=r"(current_eflags));
    
    kprintf("Current CS: %x\n", actual_cs);
    kprintf("Current EFLAGS: %x\n", current_eflags);
    kprintf("process_a at: %x\n", process_a);
    __asm__ volatile("hlt");
    uint8_t *stack = kmalloc(4096);
    uint32_t *sp = (uint32_t*)(stack + 4096);
    sp = (uint32_t*)((uint32_t)sp & ~0xF);  // Align
    
    *(--sp) = current_eflags;  // Use ACTUAL flags
    *(--sp) = actual_cs;        // Use ACTUAL cs
    *(--sp) = (uint32_t)process_b;
    
    kprintf("Jumping with ESP=%x\n", sp);
    __asm__ volatile(
        "movl %0, %%esp\n"
        "iret\n"
        : : "r"(sp)
    );
}
    */