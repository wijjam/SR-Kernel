#include "../include/process_manager.h"
#include "../include/vga.h"
#include "../include/pic.h"
#include "../include/memory.h"

struct PCB* current_process;
struct PCB* next_process;

volatile struct PCB* pcb_ai;
volatile struct PCB* pcb_bi;


void timer_interrupt_handler() {

    
    if (current_process == pcb_ai) {
        
        next_process = pcb_bi;
        //kprintf(" Current process: %d \n", current_process);
    } else {
        next_process = pcb_ai;
        // kprintf(" Current process: %d \n", current_process);
            
    }


    pic_send_eoi(0);
}

void create_process(struct PCB* pcb, void (*func)()) {
    static uint32_t next_pid = 1;
    pcb->PID = next_pid++;
    pcb->state = 1;

    // Get ACTUAL CS from CPU
    uint16_t actual_cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(actual_cs));

    uint8_t *stack = kmalloc(4096);
    uint32_t *sp = (uint32_t*)(stack + 4096);

    // IRET frame
    *(--sp) = 0x202;          // EFLAGS
    *(--sp) = actual_cs;      // CS ← ANVÄND RÄTT CS!
    *(--sp) = (uint32_t)func; // EIP

    // POPA frame
    *(--sp) = 0x00; // EAX
    *(--sp) = 0x00; // ECX
    *(--sp) = 0x00; // EDX
    *(--sp) = 0x00; // EBX
    *(--sp) = 0x00; // ESP (ignored)
    *(--sp) = 0x00; // EBP
    *(--sp) = 0x00; // ESI
    *(--sp) = 0x00; // EDI

    pcb->saved_esp = (uint32_t)sp;
}


void init_process(struct PCB* pcb_a, struct PCB* pcb_b) {
    kprintf("init_process called!\n");
    
    pcb_ai = pcb_a;
    pcb_bi = pcb_b;
}
