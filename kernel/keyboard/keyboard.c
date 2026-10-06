#include "keyboard.h"
#include "../terminal/terminal.h"

#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60

static uint8_t extended_scancode = 0;
static uint8_t shift_pressed = 0;

static uint8_t keyboard_read_scancode(void){
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
    if (shift_pressed)
    {
        switch (scancode)
        {
            case 0x02: return '!';
            case 0x03: return '@';
            case 0x04: return '#';
            case 0x05: return '$';
            case 0x06: return '%';
            case 0x07: return '^';
            case 0x08: return '&';
            case 0x09: return '*';
            case 0x0A: return '(';
            case 0x0B: return ')';
            case 0x0C: return '_';
            case 0x0D: return '+';
            case 0x33: return '<';
            case 0x34: return '>';
        }

        if (scancode >= 0x10 && scancode <= 0x19)
        {
            switch (scancode)
            {
                case 0x10: return 'Q';
                case 0x11: return 'W';
                case 0x12: return 'E';
                case 0x13: return 'R';
                case 0x14: return 'T';
                case 0x15: return 'Y';
                case 0x16: return 'U';
                case 0x17: return 'I';
                case 0x18: return 'O';
                case 0x19: return 'P';
            }
        }

        if (scancode >= 0x1E && scancode <= 0x26)
        {
            switch (scancode)
            {
                case 0x1E: return 'A';
                case 0x1F: return 'S';
                case 0x20: return 'D';
                case 0x21: return 'F';
                case 0x22: return 'G';
                case 0x23: return 'H';
                case 0x24: return 'J';
                case 0x25: return 'K';
                case 0x26: return 'L';
            }
        }

        switch (scancode)
        {
            case 0x2C: return 'Z';
            case 0x2D: return 'X';
            case 0x2E: return 'C';
            case 0x2F: return 'V';
            case 0x30: return 'B';
            case 0x31: return 'N';
            case 0x32: return 'M';
        }
    }

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
        case 0x0C: return '-';
        case 0x0D: return '=';
        case 0x33: return ',';
        case 0x34: return '.';

        default: return 0;
    }
}

void keyboard_init(void){
}

void keyboard_handler(void){
    uint8_t scancode = keyboard_read_scancode();

    /*
     * Teclas especiais usam o prefixo 0xE0.
     */
    if (scancode == 0xE0){
        extended_scancode = 1;
        return;
    }

    /*
     * Trata as teclas que vieram depois de 0xE0.
     */
    if (extended_scancode){
        extended_scancode = 0;

        /*
         * Ignora soltura das teclas especiais.
         */
        if (scancode & 0x80) return;

        if (scancode == 0x48){
            terminal_history_up();
            return;
        }

        if (scancode == 0x50){
            terminal_history_down();
            return;
        }

        return;
    }

    /*
     * Shift pressionado.
     */
    if (scancode == 0x2A || scancode == 0x36){
        shift_pressed = 1;
        return;
    }

    /*
     * Shift solto.
     */
    if (scancode == 0xAA || scancode == 0xB6){
        shift_pressed = 0;
        return;
    }

    /*
     * Ignora soltura das outras teclas.
     */
    if (scancode & 0x80) return;

    /*
     * Backspace.
     */
    if (scancode == 0x0E){
        terminal_input_backspace();
        return;
    }

    /*
     * Enter.
     */
    if (scancode == 0x1C){
        terminal_input_enter();
        return;
    }

    char character = scancode_to_ascii(scancode);

    if (character != 0)
        terminal_input_char(character);
}
