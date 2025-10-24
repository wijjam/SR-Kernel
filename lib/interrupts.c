#include "../include/interrupts.h"
#include "../include/vga.h"
#include "../include/pic.h"
#include "../include/io.h"
#include "../include/process_manager.h"

// The IDT table - 256 entries (one for each possible interrupt)
static struct idt_entry idt[256];

// IDT pointer - tells CPU where our IDT table is located
static struct idt_ptr idtp;


void set_idt_entry(uint8_t num, uint32_t handler_address, uint16_t selector, uint8_t flags) {
    idt[num].base_low = handler_address & 0xFFFF;
    idt[num].base_high = (handler_address >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void install_idt(void) {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1; // this is the size of our idt
    idtp.base = (uint32_t)&idt; // this is the address of our idt

    for (int i = 0; i < 256; i++) {
        idt[i].base_low = 0;
        idt[i].base_high = 0;
        idt[i].selector = 0;
        idt[i].always0 = 0;
        idt[i].flags = 0;
    }

    // Tells the CPU to use our IDT instead of the BIOS's
    __asm__ volatile ("lidt %0" : : "m" (idtp));

}

void init_interrupts(void) {
    kprintf("Setting up interrupts....\n");
    
    // Check current code segment
    uint16_t cs;
    __asm__ volatile ("mov %%cs, %0" : "=r" (cs));
    kprintf("Current code segment: %d\n", cs);
    
    install_idt();
    setup_time(11932); // makes it so we get 1 interrupt per 10ms
    
    // Use the actual current code segment instead of assuming 0x08
    set_idt_entry(129, (uint32_t)isr_wrapper_129, cs, 0x8E); // The system call interrupt
    set_idt_entry(33, (uint32_t)isr_wrapper_33, cs, 0x8E);
    set_idt_entry(32, (uint32_t)isr_wrapper_32, cs, 0x8E);
}

void setup_time(uint16_t divisor) {

    outb(0x43, 0x36); // We are telling the PIT to use Channel 0 which is the timer Channel

    outb(0x40, (divisor & 0x00FF)); // Just compare the right most number aka the low bit.
    outb(0x40, (divisor >> 8) & 0x00FF);  // Shift right and compare getting the top bit.

}