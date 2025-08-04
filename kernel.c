#include <stdarg.h>
#include <stdint.h>
#include "include/io.h"
#include "include/rtc.h"
#include "include/vga.h"
#include "include/interrupts.h"
#include "include/pic.h"
#include "include/keyboard.h"
#include "include/commands.h"
#include "include/memory.h"









void kernel_main(void) {
    // In your main function:
    kprintf("Setting up PIC...\n");
    pic_remap(32, 40);  // Remap IRQs: Master to 32-39, Slave to 40-47
    pic_disable_irq(0); // Disable timer for now (would overwhelm us)
    pic_enable_irq(1);  // Enable keyboard
    kprintf("PIC configured: Timer disabled, Keyboard enabled\n");
    
    
    kprintf("=== Setting up interrupt system ===\n");
    
    // Step 1: Set up interrupt infrastructure
    init_interrupts();
    
    // Step 2: Configure PIC (remap and set up IRQ masks)
    kprintf("Configuring PIC...\n");
    pic_remap(32, 40);        // IRQ 0-7 → interrupts 32-39, IRQ 8-15 → interrupts 40-47
    pic_disable_irq(0);       // Disable timer (would overwhelm us)
    pic_enable_irq(1);        // Enable keyboard
    kprintf("PIC configured\n");
    
    // Step 3: Initialize keyboard driver
    heap_init();
    init_keyboard();
    init_char_table();
    // Step 4: Test software interrupt first
    kprintf("Testing software interrupt...\n");
    test_software_interrupt();
    
    kprintf("=== Interrupt system ready ===\n");
    kprintf("Global interrupts still DISABLED for safety\n");
    kprintf("Press keys - nothing will happen yet\n");
    
    kprintf("=== Interrupt system ready ===\n");
    
    // THE MOMENT OF TRUTH
    kprintf("\n🚀 ENABLING GLOBAL INTERRUPTS IN 3...\n");
    // Add small delay so you can read it
    for (volatile int i = 0; i < 10000000; i++);
    
    kprintf("2...\n");
    for (volatile int i = 0; i < 10000000; i++);
    
    kprintf("1...\n");
    for (volatile int i = 0; i < 10000000; i++);
    
    kprintf("🔥 ENABLING INTERRUPTS NOW!\n");
    
    // Enable global interrupts
    __asm__ volatile ("sti");
    
    kprintf("✅ INTERRUPTS ENABLED! Press keys now!\n");
    kprintf("(If you can read this, we didn't crash!)\n");

    // Main kernel loop - just wait for interrupts
    while (1) {
        // Do nothing - let interrupts handle everything
        // This is where your kernel "idles"
    }

}