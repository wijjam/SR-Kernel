#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#define SLEEP_SYSCALL_MAGIC 555555


#include <stdint.h>

typedef struct PCB {
    uint32_t saved_esp;
    uint16_t PID;
    int sleep_time;

};
extern struct PCB* current_process;
extern struct PCB* next_process;


void create_process(void (*function_address)());

void sleep(int time);

void init_process_scheduler(void);


#endif