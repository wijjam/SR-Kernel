#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <stdint.h>

#define EAX_INDEX 0 // Handles basic arithmatic
#define EBX_INDEX 1 // Holds the address for important data like the first index of an array or string.
#define ECX_INDEX 2 // The counter, saves for example how far in a for loop we have gone.
#define EDX_INDEX 3 // This register holds the data that "overflows" from EAX. Also used for I/O
#define ESI_INDEX 4 // Points to where data COMES FROM
#define EDI_INDEX 5 // Points to where data ENDS ON
#define ESP_INDEX 6 // Points to the top of the stack which holds all the local variables
#define EBP_INDEX 7 // Tracks where the start of functions are in the stack. AKA the bounds of the function
#define EIP_INDEX 8 // VERY IMPORTANT: Points to the next instruction that is going to be read.


typedef struct switch_registers {
    int reg[9];
};

void switch_process();




#endif