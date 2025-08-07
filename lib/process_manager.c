#include "../include/process_manager.h"



void switch_process() {

    struct switch_registers registers;
    asm volatile ("mov %%eax, %0" : "=m" (registers.reg[EAX_INDEX]));
    asm volatile("mov %%ebx, %0" : "=m" (registers.reg[EBX_INDEX]));
    asm volatile("mov %%ecx, %0" : "=m" (registers.reg[ECX_INDEX]));
    asm volatile("mov %%edx, %0" : "=m" (registers.reg[EDX_INDEX]));
    asm volatile("mov %%esi, %0" : "=m" (registers.reg[ESI_INDEX]));
    asm volatile("mov %%edi, %0" : "=m" (registers.reg[EDI_INDEX]));
    asm volatile("mov %%esp, %0" : "=m" (registers.reg[ESP_INDEX]));
    asm volatile("mov %%ebp, %0" : "=m" (registers.reg[EBP_INDEX]));
    asm volatile("call 1f\n"
                 "1: pop %0"
                 : "=r" (registers.reg[EIP_INDEX]));

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