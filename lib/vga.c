#include "../include/vga.h"
#include <stdarg.h>

volatile char* video_memory = (volatile char*)0xB8000;
int cursor_row = 0;
int cursor_col = 0;
char color = 0x0F;


char make_color(char fg, char bg) {
    return fg | (bg << 4);
}

void set_color(char fg, char bg) {
    color = make_color(fg, bg);
}

void print_char(char c) {
    int position = 0;
    
    if (cursor_row >= 25) {
    // Move each row up by 160 bytes (one row)
        for (int row = 0; row < 24; row++) {
        // Copy from row+1 to row
            int source_start = (row + 1) * 160;
            int dest_start = row * 160;
        
        // Copy all 160 bytes of this row
            for (int byte = 0; byte < 160; byte++) {
                video_memory[dest_start + byte] = video_memory[source_start + byte];
            }
        }

        for (int j = 0; j < 80; j++) {
            position = (24 * 80 + j) * 2;
            video_memory[position] = ' ';
            video_memory[position + 1] = color;
        }
    
        cursor_row = 24; // Now this runs once, after scrolling
        cursor_col = 0;
    } 
    
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
    } else {
        position = (cursor_row * 80 + cursor_col) * 2;
        video_memory[position] = c;
        video_memory[position + 1] = color;
        cursor_col++;

        if (cursor_col >= 80) {
                cursor_col = 0;
                cursor_row++;
            }
    }
}

void print(const char* message) {

    for (int i = 0; message[i] != '\0'; i++) {

        print_char(message[i]);

    }
}

char* int_to_string(int value) {
    static char string[12];  // -2147483648 is 11 chars + null terminator
    int i = 0;
    int is_negative = 0;
    
    // Handle negative numbers
    if (value < 0) {
        is_negative = 1;
        // Handle INT_MIN case (-2147483648) which can't be negated safely
        if (value == -2147483648) {
            string[0] = '-'; string[1] = '2'; string[2] = '1'; string[3] = '4';
            string[4] = '7'; string[5] = '4'; string[6] = '8'; string[7] = '3';
            string[8] = '6'; string[9] = '4'; string[10] = '8'; string[11] = '\0';
            return string;
        }
        value = -value;
        string[i++] = '-';
    }
    
    // Handle zero case
    if (value == 0) {
        string[i++] = '0';
        string[i] = '\0';
        return string;
    }
    
    // Find where to start placing digits
    int start_pos = i;
    
    // Convert digits (they'll be in reverse order)
    while (value > 0) {
        string[i++] = (value % 10) + '0';
        value /= 10;
    }
    
    // Null terminate
    string[i] = '\0';
    
    // Reverse the digit portion only
    int end_pos = i - 1;
    while (start_pos < end_pos) {
        char temp = string[start_pos];
        string[start_pos] = string[end_pos];
        string[end_pos] = temp;
        start_pos++;
        end_pos--;
    }
    
    return string;
}

char* double_to_string(double value) {
    static char string[64];  // Max length for a double representation
    int i = 0;
    int decimal_pos;
    int is_negative = 0;
    
    // Handle negative numbers
    if (value < 0) {
        is_negative = 1;
        value = -value;
        string[i++] = '-';
    }
    
    // Handle zero case
    if (value == 0.0) {
        string[i++] = '0';
        string[i++] = '.';
        string[i++] = '0';
        string[i] = '\0';
        return string;
    }
    
    // Extract integer part
    long long int_part = (long long)value;
    double frac_part = value - int_part;
    
    // Convert integer part to string (reverse order first)
    int start_pos = i;
    if (int_part == 0) {
        string[i++] = '0';
    } else {
        while (int_part > 0) {
            string[i++] = '0' + (int_part % 10);
            int_part /= 10;
        }
        
        // Reverse the integer part
        int end_pos = i - 1;
        while (start_pos < end_pos) {
            char temp = string[start_pos];
            string[start_pos] = string[end_pos];
            string[end_pos] = temp;
            start_pos++;
            end_pos--;
        }
    }
    
    // Add decimal point
    string[i++] = '.';
    
    // Convert fractional part (up to 6 decimal places)
    int decimal_places = 6;
    while (decimal_places > 0 && frac_part > 0) {
        frac_part *= 10;
        int digit = (int)frac_part;
        string[i++] = '0' + digit;
        frac_part -= digit;
        decimal_places--;
    }
    
    // If no fractional part was added, add at least one zero
    if (string[i-1] == '.') {
        string[i++] = '0';
    }
    
    string[i] = '\0';
    return string;
}



void kprintf(char* start_text, ...){
    va_list args;
    va_start(args, start_text);

    while (*start_text != '\0') {
        
        if (*start_text == '%') {
            start_text = start_text + 1;
            
            switch(*start_text) {

                case 's':
                    char* string = va_arg(args, char*);
                    print(string);
                break;

                case 'c':
                    char character = (char)va_arg(args, int);
                    print_char(character);
                break;

                case 'd':
                    int value = va_arg(args, int);
                    char* s_value = int_to_string(value);
                    print(s_value);
                break;

                case 'f':
                    double d_value = va_arg(args, double);
                    char* sd_value = double_to_string(d_value);
                    print(sd_value);
                break;

                case 'e':
                    color = 0x04;
                    start_text++;
                    print(start_text);
                    color = 0x0F;
                    return;
                break;



            }

        } else {
            print_char(*start_text);
        }

        start_text = start_text + 1;
    }


    va_end(args);
}

void clearScreen() {
    int position = 0;
    for (int row = 0; row < 25; row++) {
        for (int col = 0; col < 80; col++) {
            int position = (row * 80 + col) * 2;
            video_memory[position] = ' ';
            video_memory[position + 1] = 0x07; // Light gray on black
        }
    }
    // Reset cursor to top-left
    cursor_row = 0;
    cursor_col = 0;
}


void backspace() {
    if (cursor_col > 0) { // Checks that the cursos is not touching the left end
        cursor_col--; // Moves back one space
        print_char(' '); // Removes the space infront of it
        cursor_col--; // Moves back one space to write on the newly deleted space
    }

}