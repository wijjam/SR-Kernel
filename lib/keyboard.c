#include "../include/keyboard.h"
#include "../include/io.h"
#include "../include/pic.h"
#include "../include/vga.h"
#include "../include/keymap.h"
#include "../include/commands.h"
#include "../include/memory.h"

static int left_shift_pressed = 0;
static int right_shift_pressed = 0;

#define LEFT_SHIFT_PRESS    0x2A
#define LEFT_SHIFT_RELEASE  0xAA
#define RIGHT_SHIFT_PRESS   0x36
#define RIGHT_SHIFT_RELEASE 0xB6

static char command_buffer[256];
static int buffer_pos = 0;

void keyboard_interrupt_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);
    
    // Handle shift key state tracking
    switch(scancode) {
        case LEFT_SHIFT_PRESS:
            left_shift_pressed = 1;
            break;
        case LEFT_SHIFT_RELEASE:
            left_shift_pressed = 0;
            break;
        case RIGHT_SHIFT_PRESS:
            right_shift_pressed = 1;
            break;
        case RIGHT_SHIFT_RELEASE:
            right_shift_pressed = 0;
            break;
        default:
            // Handle regular keys
            if (!(scancode & 0x80)) {
                // Key pressed
                int shift_active = left_shift_pressed || right_shift_pressed;
                char ascii = scancode_to_ascii(scancode, shift_active);
                if (ascii != 0) {
                    if (ascii == '\n') {
                        command_buffer[buffer_pos] = '\0';  // Null terminate
                        process_input(command_buffer);    // Process the command
                        buffer_pos = 0;                     // Reset for next command
                         kprintf("\n> ");                    // Show prompt
                    } else if (ascii == '\b') {
                        // Handle backspace
                        backspace();
                    if (buffer_pos > 0) {
                        buffer_pos--;
                     // Move cursor back and clear character
    }
                    } else if (ascii != 0) {
                        command_buffer[buffer_pos++] = ascii;
                        kprintf("%c", ascii);  // Echo the character
            }  // Just print the character
                }
            }
            // Ignore key releases for regular keys
            break;
    }
    
    pic_send_eoi(1);
}

void init_keyboard(void) {
    kprintf("Keyboard driver initialized\n");
    // Nothing special needed for basic keyboard
    // Hardware is already configured by BIOS
}