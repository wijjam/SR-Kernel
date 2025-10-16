#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdint.h>


typedef struct PCB {
    uint32_t saved_esp;
    uint32_t PID;
    uint32_t state;
};
extern struct PCB* current_process;
extern struct PCB* next_process;

void init_process(struct PCB* pcb_a, struct PCB* pcb_b);

void create_process(struct PCB* pcb, void (*function_address)());


#endif