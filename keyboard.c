#include "keyboard.h"
#include "../terminal/terminal.h"

#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

static uint8_t keyboard_read_scancode(void)
{
    uint8_t scancode;

    __asm__ volatile (
        "inb %1, %0"
        : "=a"(scancode)
        : "Nd"((uint16_t)KEYBOARD_DATA_PORT)
    );

    return scancode;
}

static char scancode_to_ascii(uint8_t scancode)
{
    switch (scancode)
    {
        case 0x02: return '1';
        case 0x03: return '2';
        case 0x04: return '3';
        case 0x05: return '4';
        case 0x06: return '5';
        case 0x07: return '6';
        case 0x08: return '7';
        case 0x09: return '8';
        case 0x0A: return '9';
        case 0x0B: return '0';

        case 0x10: return 'q';
        case 0x11: return 'w';
        case 0x12: return 'e';
        case 0x13: return 'r';
        case 0x14: return 't';
        case 0x15: return 'y';
        case 0x16: return 'u';
        case 0x17: return 'i';
        case 0x18: return 'o';
        case 0x19: return 'p';

        case 0x1E: return 'a';
        case 0x1F: return 's';
        case 0x20: return 'd';
        case 0x21: return 'f';
        case 0x22: return 'g';
        case 0x23: return 'h';
        case 0x24: return 'j';
        case 0x25: return 'k';
        case 0x26: return 'l';

        case 0x2C: return 'z';
        case 0x2D: return 'x';
        case 0x2E: return 'c';
        case 0x2F: return 'v';
        case 0x30: return 'b';
        case 0x31: return 'n';
        case 0x32: return 'm';

        case 0x39: return ' ';

        default:
            return 0;
    }
}

void keyboard_init(void)
{
}

void keyboard_handler(void)
{
    uint8_t scancode =
        keyboard_read_scancode();

    if (scancode & 0x80)
        return;

    if (scancode == 0x0E)
    {
        terminal_input_backspace();
        return;
    }

    if (scancode == 0x1C)
    {
        terminal_input_enter();
        return;
    }

    char character =
        scancode_to_ascii(scancode);

    if (character != 0)
        terminal_input_char(character);
}