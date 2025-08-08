#include "../include/process_manager.h"

struct switch_registers registers;



void timer_interrupt_handler() {


    //                  START OF SAVE SEGMENT
    // ==================================================================
    asm volatile ("mov %%eax, %0" : "=m" (registers.reg[EAX_INDEX]));
    asm volatile("mov %%ebx, %0" : "=m" (registers.reg[EBX_INDEX]));
    asm volatile("mov %%ecx, %0" : "=m" (registers.reg[ECX_INDEX]));
    asm volatile("mov %%edx, %0" : "=m" (registers.reg[EDX_INDEX]));
    asm volatile("mov %%esi, %0" : "=m" (registers.reg[ESI_INDEX]));
    asm volatile("mov %%edi, %0" : "=m" (registers.reg[EDI_INDEX]));
    asm volatile("mov %%ebp, %0" : "=m" (registers.reg[EBP_INDEX]));
    asm volatile("mov %%esp, %0" : "=m" (registers.reg[ESP_INDEX])); 
    // Save the stack last of the general registers
    // Reason is some registers before this need to be saved and at the end we get the full. SO we go from small, induvidual
    // values and saves to then save the entier thing.

     // Remove the reigster values EIP, CS and EFLAGS that CPU automaticaly sends to stack to replace with my values.
     // pop while saving saves instructions and also makes sure in the load part the CPU's values are gone from the stack so
     // we just need to push our values without having to think about the cpu.
    asm volatile("pop %0" : "=m" (registers.reg[EIP_INDEX]));
    asm volatile("pop %0" : "=m" (registers.reg[CS_INDEX]));
    asm volatile("pop %0" : "=m" (registers.reg[EFLAGS_INDEX]));
    


                 
    
    //                  START OF LOAD SEGMENT
    // ==================================================================

    pic_send_eoi(0); // Tell the PIC we have handled the interrupt. Ready to catch more.
                     // CPU sees it is still in interrupt mode, so it wants no interrupts yet.

    // We push all these 3 values on to the stack in the order the CPU reads them. Essencialy replacing them.
    
    
    asm volatile ("mov %0, %%esp" : : "m" (registers.reg[ESP_INDEX])); // Important to keep this at the top here.
                                                                       // Registers below will use this new stack. Otherwise we would
                                                                       // load new values into registers and the current stack and then change the stack.
                                                                       // This woild lead to a crash.

    asm volatile("push %0" : : "m" (registers.reg[EFLAGS_INDEX])); 
    asm volatile("push %0" : : "m" (registers.reg[CS_INDEX]));
    asm volatile("push %0" : : "m" (registers.reg[EIP_INDEX]));
    asm volatile ("mov %0, %%eax" : : "m" (registers.reg[EAX_INDEX]));
    asm volatile ("mov %0, %%ebx" : : "m" (registers.reg[EBX_INDEX]));
    asm volatile ("mov %0, %%ecx" : : "m" (registers.reg[ECX_INDEX]));
    asm volatile ("mov %0, %%edx" : : "m" (registers.reg[EDX_INDEX]));
    asm volatile ("mov %0, %%esi" : : "m" (registers.reg[ESI_INDEX]));
    asm volatile ("mov %0, %%edi" : : "m" (registers.reg[EDI_INDEX]));
    asm volatile ("mov %0, %%ebp" : : "m" (registers.reg[EBP_INDEX]));
    // Do not use iret since assembly wrapper uses it.
}


void printReg(uint8_t type, int reg[]) {

    switch(type) {
        case 0: kprintf("The current value in EAX: %d\n", reg[EAX_INDEX]);
        break;
        case 1: kprintf("The current value in EBX: %d\n", reg[EBX_INDEX]);
        break;
        case 2: kprintf("The current value in ECX: %d\n", reg[ECX_INDEX]);
        break;
        case 3: kprintf("The current value in EDX: %d\n", reg[EDX_INDEX]);
        break;
        case 4: kprintf("The current value in ESI: %d\n", reg[ESI_INDEX]);
        break;
        case 5: kprintf("The current value in EDI: %d\n", reg[EDI_INDEX]);
        break;
        case 6: kprintf("The current value in ESP: %d\n", reg[ESP_INDEX]);
        break;
        case 7: kprintf("The current value in EBP: %d\n", reg[EBP_INDEX]);
        break;
        case 8: kprintf("The current value in EIP: %d\n", reg[EIP_INDEX]);
        break;
    }


}