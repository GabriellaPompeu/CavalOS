#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>
#include "../filesystem/filesystem.h"

void terminal_init(void);

filesystem* terminal_get_filesystem(void);

void terminal_putchar(char character);
void terminal_write_string(const char* data);
void terminal_printf(const char* format, ...);

void terminal_input_char(char character);
void terminal_input_enter(void);
void terminal_input_backspace(void);
//void terminal_clear(void);

#endif
