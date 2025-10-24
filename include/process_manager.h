#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdint.h>

typedef struct PCB {
    uint32_t saved_esp;
    uint16_t PID;
    uint16_t sleep_time;

};
extern struct PCB* current_process;
extern struct PCB* next_process;


void create_process(void (*function_address)());

void sleep(int time);


#endif