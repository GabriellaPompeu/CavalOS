#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#if defined(__linux__)
#endif

#if !defined(__i386__)
#error "Tem q compilar em ix86-elf filho..."
#endif

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

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
	WHITE = 15,
};

static inline uint8_t vga_entry_color(enum vga_color foreground, enum vga_color background){
    return foreground | background << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color){
    return (uint16_t) uc | (uint16_t) color << 8;
}

void terminal_initialize(){
	terminal_color = vga_entry_color(LIGHT_GREY, BLACK);
	terminal_row = 0;
	terminal_column = 0;

	for (size_t y = 0; y < VGA_HEIGHT; y++){ /*percorre linhas*/
		for (size_t x = 0; x < VGA_WIDTH; x++){ /*percorre colunas*/
			const size_t i = y * VGA_WIDTH + x; /*transformo linha/coluna em posição*/
			terminal_buffer[i] = vga_entry(' ', terminal_color); /*limpamos a tela*/	
		}
	}
}

void terminal_setcolor(uint8_t color){
	terminal_color = color;
}

void terminal_scroll(){
	for (size_t y = VGA_HEIGHT - 1; y > 0; y--){
		for (size_t x = 0; x < VGA_WIDTH; x++){
			const size_t origem = y * VGA_WIDTH + x;
			const size_t destino = (y-1) * VGA_WIDTH + x;
			
			terminal_buffer[destino] = terminal_buffer[origem];
		}
	}

	for (size_t x = 0; x < VGA_WIDTH; x++){
		const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
		terminal_buffer[index] = vga_entry(' ', terminal_color);
	}
}

/*Escreve o caractere em uma posição específica*/
void terminal_put_entry_at(char character, uint8_t color, size_t x, size_t y){
	const size_t i = y * VGA_WIDTH + x;
	terminal_buffer[i] = vga_entry(character, color);
}

/*Escreve o caractere na posição atual*/
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

void terminal_write(const char* data, size_t size){
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_write_string(const char* data){
	terminal_write(data, strlen(data));
}

void kernel_main(){
	const char* CAVALOS =
	"   _____                 _  ____   _____  \n"
	"  / ____|               | |/ __ \\ / ____| \n"
	" | |     __ ___   ____ _| | |  | | (___   \n"
	" | |    / _` \\ \\ / / _` | | |  | |\\___ \\  \n"
	" | |____ (_| |\\ V / (_| | | |__| |____) | \n"
	"  \\_____\\__,_| \\_/ \\__,_|_|\\____/|_____/\n\n";
	
	const char* DEVS = "\nDesenvolvido por Bruna Luiza, Daniel Pita,\n"
	"Felipe Dutra, Gabriella Pompeu e Raynner Meza.\n";

	terminal_initialize();
	terminal_write_string("==================================================\n");

	terminal_write_string("          Bem-vindo ao CavalOS!\n");
	terminal_write_string(CAVALOS);
	terminal_write_string(DEVS);

	terminal_write_string("==================================================\n");
}