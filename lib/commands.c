#include "../include/commands.h"
#include "../include/vga.h"
#include "../include/rtc.h"
#include "../include/memory.h"
#include "../include/process_manager.h"
#include <stdarg.h>


void process_input(char* input) {


    

    if (checkString(input, "help")) {
        cmd_help();
    } else if (checkString(input, "neofetch")) {
        kprintf("%eYou called neofetch my guy!!!\n");
    } else if (checkString(input, "mem")) {
        test_kmalloc_kfree();
        
    } else if (checkString(input, "time")) {
        print_rtc_time();
    } else if (checkString(input, "clear")) {
        clearScreen();
    } else if (checkString(input, "debug")) {
        kprintf("BANAN");
    }
    else {
        kprintf("\n%eCommand not found\n");
    }

}
int checkString(char* input, char* compareString) {
    
    int i = 0;
    while (input[i] != '\0' || compareString[i] != '\0') {
        if (input[i] != compareString[i]) {
            return 0;
        }
        i++;
    }
    return 1;
}








void cmd_help(void) {
    kprintf("To calaculate number write in format 'calc num operation num'\n");
    kprintf("To get the awsome image write neofetch'\n");
    
}