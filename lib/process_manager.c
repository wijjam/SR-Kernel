#include "../include/process_manager.h"
#include "../include/vga.h"
#include "../include/pic.h"
#include "../include/memory.h"

struct PCB* current_process;
struct PCB* next_process;

struct PCB process_lists[1000];
uint32_t current_index = 0;

void timer_interrupt_handler(uint32_t esp_address_variable) {
            
        // The round robbin functionality

        uint32_t *stack = (uint32_t *)esp_address_variable; 

    if (stack[18] != SLEEP_SYSCALL_MAGIC) {
        for (int i = 0; i < current_index; i++) {
            if (process_lists[i].sleep_time > 0) {
                process_lists[i].sleep_time = process_lists[i].sleep_time - 1;
            }
        }
    }

        int current_idx = (current_process - process_lists);
        int next_idx = (current_idx + 1) % current_index;
        

        // Keep looking until we find one that's awake (or hit idle process 0)
        while (process_lists[next_idx].sleep_time > 0 && next_idx != 0) {
            next_idx = (next_idx + 1) % current_index;
        }
        
        next_process = &process_lists[next_idx];

 //kprintf("This is 10ms=======================================================================\n");

    pic_send_eoi(0);
}

void create_process(void (*func)()) {
    process_lists[current_index].PID = (uint16_t)current_index; // Gives the process a PID that is explicitly its own.
    process_lists[current_index].sleep_time = 0; // assings the sleep time to 0 for the process being created.
    // Get ACTUAL CS from CPU
    uint16_t actual_cs;
    __asm__ volatile("mov %%cs, %0" : "=r"(actual_cs)); // Gets the CS from the cpu, no Assumsion!

    uint8_t *stack = kmalloc(4096); // We allocate our custom stack frame to the heap.
    if (stack == (void*)0) {return;} // we then check if it returns null, if the heap runs out we can not allocate a stack.
    uint32_t *sp = (uint32_t*)(stack + 4096); // Since stack moves downward we make the stack pointer (esp) point at the top of the heap and then move down.

    // IRET frame
    *(--sp) = 0x202;          // EFLAGS
    *(--sp) = actual_cs;      // CS <- ANVÄND RÄTT CS!
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

    process_lists[current_index].saved_esp = (uint32_t)sp;
    current_index++;
}

void init_process_scheduler(void) {
    current_process = &process_lists[0];
    next_process = &process_lists[0];
}


void sleep(int time) {

    current_process->sleep_time = time;

    __asm__ volatile ("movl $555555, 28(%%esp) \n\t" 
                      "int $0x20"
        : 
        : 
        :             "%eax"
    );

    

}

void system_call_interrupt_handler(uint32_t esp_address_variable) {

    //kprintf("Successfully called a system call!!!\n");
    uint32_t *stack = (uint32_t *)esp_address_variable; 



    if (stack[18] == 2) {
        //kprintf("HELLO WORLD!!!!!\n");
    }


    //kprintf("The eax you called had this: %d\n", stack[18]);




}


