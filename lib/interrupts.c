#include "../include/interrupts.h"
#include "../include/vga.h"

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
    
    // Use the actual current code segment instead of assuming 0x08
    set_idt_entry(80, (uint32_t)isr_wrapper_80, cs, 0x8E);
    set_idt_entry(33, (uint32_t)isr_wrapper_33, cs, 0x8E);
    
    kprintf("Interrupts installed: 80=test, 33=keyboard\n");
}


void test_interrupt_handler(void) {
    kprintf("Interrupt fired! It works yay.\n");
}

void test_software_interrupt(void) {
    kprintf("About to trigger interrupt 80...\n");
    __asm__ volatile ("int $80");
    kprintf("Back from interrupt - it worked!\n");
}