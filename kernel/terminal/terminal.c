#include "terminal.h"
#include "../timer/timer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

extern uint32_t pmm_get_total_frames(void);
extern uint32_t pmm_get_used_frames(void);
extern uint32_t pmm_get_free_frames(void);
extern void reboot(void);

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

#define INPUT_BUFFER_SIZE 128

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;

static uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

static char input_buffer[INPUT_BUFFER_SIZE];
static size_t input_length = 0;

enum vga_color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15
};

static inline uint8_t vga_entry_color(enum vga_color foreground, enum vga_color background){
    return foreground | background << 4;
}

static inline uint16_t vga_entry(unsigned char character, uint8_t color){
    return (uint16_t)character | (uint16_t)color << 8;
}

static void terminal_put_entry_at(char character, uint8_t color, size_t x, size_t y){
    const size_t index = y * VGA_WIDTH + x;

    terminal_buffer[index] = vga_entry(character, color);
}

static void terminal_scroll(void){
    for (size_t y = 1; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            const size_t origem =
                y * VGA_WIDTH + x;

            const size_t destino =
                (y - 1) * VGA_WIDTH + x;

            terminal_buffer[destino] =
                terminal_buffer[origem];
        }
    }

    for (size_t x = 0; x < VGA_WIDTH; x++){
        const size_t index =
            (VGA_HEIGHT - 1) * VGA_WIDTH + x;

        terminal_buffer[index] =
            vga_entry(' ', terminal_color);
    }
}

void terminal_putchar(char character){
    if (character == '\n'){
        terminal_column = 0;

        if (++terminal_row == VGA_HEIGHT){
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }

        return;
    }

    terminal_put_entry_at(character, terminal_color, terminal_column, terminal_row);

    if (++terminal_column == VGA_WIDTH){
        terminal_column = 0;

        if (++terminal_row == VGA_HEIGHT){
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
    }
}

void terminal_write_string(const char* data){
    for (size_t i = 0; data[i] != '\0'; i++)
        terminal_putchar(data[i]);
}

static void terminal_write_int(int value){
    char buffer[12];
    size_t i = 0;

    if (value == 0){
        terminal_putchar('0');
        return;
    }

    if (value < 0){
        terminal_putchar('-');
        value = -value;
    }

    while (value > 0){
        buffer[i++] =
            '0' + (value % 10);

        value /= 10;
    }

    while (i > 0) terminal_putchar(buffer[--i]);
}

void terminal_printf(const char* format, ...){
    va_list args;

    va_start(args, format);

    for (size_t i = 0; format[i] != '\0'; i++){
        if (format[i] != '%'){
            terminal_putchar(format[i]);
            continue;
        }

        i++;

        switch (format[i]){
            case 'c':{
                char character =
                    (char)va_arg(args, int);

                terminal_putchar(character);

                break;
            }

            case 's':{
                const char* string =
                    va_arg(args, const char*);

                terminal_write_string(string);

                break;
            }

            case 'd':{
                int value =
                    va_arg(args, int);

                terminal_write_int(value);

                break;
            }

            case '%':{
                terminal_putchar('%');

                break;
            }

            default:{
                terminal_putchar('%');
                terminal_putchar(format[i]);

                break;
            }
        }
    }

    va_end(args);
}

void terminal_input_char(char character){
    if (input_length >= INPUT_BUFFER_SIZE - 1) return;

    input_buffer[input_length] = character;

    input_length++;

    terminal_putchar(character);
}

void terminal_input_backspace(void){
    if (input_length == 0) return;

    input_length--;

    if (terminal_column > 0) terminal_column--;

    terminal_put_entry_at(' ', terminal_color, terminal_column, terminal_row);
}

static void terminal_clear(void){
    terminal_row = 0;
    terminal_column = 0;

    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH; x++){
            terminal_buffer[y * VGA_WIDTH + x] = vga_entry(' ', terminal_color);
        }
    }
}

static void terminal_execute_command(void){
    if (input_length == 0) return;

    if (strcmp(input_buffer, "help") == 0){
        terminal_write_string("\nComandos disponiveis:\n");
        terminal_write_string("help - mostra esta mensagem\n");
        terminal_write_string("clear - limpa a tela\n");
        terminal_write_string("ticks - mostra os ticks do timer\n");
        terminal_write_string("uptime - mostra quanto tempo o sistema esta ligado\n");
        terminal_write_string("mem - mostra status da memoria\n");
        terminal_write_string("reboot - reinicia o sistema");

    }else if (strcmp(input_buffer, "clear") == 0){
        terminal_clear();

    }else if (strcmp(input_buffer, "ticks") == 0){
        terminal_write_string("Ticks: ");
        terminal_printf("%d\n", timer_get_ticks());

    }else if (strcmp(input_buffer, "uptime") == 0){

        uint32_t ticks = timer_get_ticks();
        uint32_t segundos = ticks / 100;
        terminal_printf("Sistema ligado ha %d segundos.\n", segundos);

    }else if (strcmp(input_buffer, "mem") == 0){

        terminal_printf("Total de frames: %d\n", pmm_get_total_frames());
        terminal_printf("Frames ocupados: %d\n", pmm_get_used_frames());
        terminal_printf("Frames vagos: %d\n", pmm_get_free_frames());

    }else if(strcmp(input_buffer, "reboot") == 0){
        reboot();

    }else{
        terminal_write_string("\nComando nao encontrado.\n");
    }
}

void terminal_input_enter(void){
    input_buffer[input_length] = '\0';

    terminal_putchar('\n');

    terminal_execute_command();
    /** Por enquanto não executamos
     * o comando.*/

    input_length = 0;

    terminal_write_string("CavalOS> ");
}

void terminal_init(void){
    terminal_color = vga_entry_color(LIGHT_GREY, BLACK);

    terminal_row = 0;
    terminal_column = 0;

    input_length = 0;

    for (size_t y = 0; y < VGA_HEIGHT; y++){
        for (size_t x = 0; x < VGA_WIDTH;x++){
            const size_t index =y * VGA_WIDTH + x;

            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}
