#include "../include/process_manager.h"

volatile int need_reschedule = 0;

void sleep(int time) {
    asm volatile(
        "movl $1, %%eax\n"        // Syscall number: 1 = sleep
        "movl %0, %%ebx\n"        // Argument: time
        "int $0x81\n"             // Call syscall interrupt
        :
        : "r"(time)
        : "eax", "ebx"
    );
}


int fork() {

        asm volatile(
        "movl $2, %%eax\n"        // Syscall number: 2 = fork
        "int $0x81\n"             // Call syscall interrupt
        :
        : 
        : "eax"
    );
    


}



void system_call_interrupt_handler(uint32_t* stack) {

    struct registers* regs = (struct registers*)stack;


    switch (regs->eax) {
        case 1:
            current_process->sleep_time = regs->ebx;
            schedule();
            need_reschedule = 1;
        break;

        case 2:
            
    }
    

}