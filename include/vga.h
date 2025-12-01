#ifndef VGA_H
#define VGA_H
#include <stdint.h>
char make_color(char fg, char bg);
void set_color(char fg, char bg);
void print_char(char c);
void print(const char* message);
char* int_to_string(int value);
char* double_to_string(double value);
void kprintf(char* start_text, ...);
void clearScreen();
void backspace();
void update_print_corner_time();

#endif