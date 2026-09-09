#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

#if defined(__linux__)
#error "Embora pareça, isso aqui n é Linux n..."
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
	for (size_t y = 1; y < VGA_HEIGHT; y++){
		for (size_t x = 0; x < VGA_WIDTH; x++){
			const size_t origem = y * VGA_WIDTH + x;
			const size_t destino = (y - 1) * VGA_WIDTH + x;

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

static void terminal_write_int(int value){
	char buffer[12];
	size_t i = 0;

	if (value == 0){
		terminal_putchar('0');
		return;
	} else {
		while (value > 0){
			buffer[i++] = '0' + (value % 10); /*transforma o digito numerico no caractere correspondente pq em ASCII '0' = 48*/
			value /= 10;
		}
	
		while (i > 0) terminal_putchar(buffer[--i]);
	}
}

void terminal_printf(const char* format, ...){
	va_list args;
	va_start(args, format);

	for (size_t i = 0; format[i] != '\0'; i++){
		if (format[i] != '%'){
			terminal_putchar(format[i]);
			continue;
		} else {
			i++;
			
			switch (format[i]){
				case 'c':{
					char c = (char)va_arg(args, int);
					terminal_putchar(c);
					break;
				}
				case 's':{
					const char* str = va_arg(args, const char*);
					terminal_write_string(str);
					break;
				}
				case 'd':{
					int value = va_arg(args, int);
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
	}

	va_end(args);
}


/*constructor_t = nome q criamos para representar um ponteiro para uma função sem argumentos e sem retorno*/
typedef void (*constructor_t)(void);

/*extern = diz pro compilador q isso existe mas n foi definido neste arquivo (no nosso caso, foi definido no linker)*/
extern constructor_t __ctors_start[];
extern constructor_t __ctors_end[];

/*percorre a região dos constructors e chama cada função q encontrar lá*/
static void call_global_constructors(void){
	size_t count = __ctors_end - __ctors_start;
	for (size_t i = 0; i < count; i++) __ctors_start[i]();
}

static int constructor_test = 0;

__attribute__((constructor)) /*informa ao GCC q isso deve ser tratado como um construtor*/
static void teste_constructor(void){
	constructor_test = 42;
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

	call_global_constructors();

	terminal_initialize();
	
	terminal_write_string("==================================================\n");
	
	terminal_write_string("          Bem-vindo ao CavalOS!\n");
	terminal_write_string(CAVALOS);
	terminal_write_string(DEVS);
	
	terminal_write_string("==================================================\n");

	uint8_t buffer[5];
	memset(buffer, 'A', 5);

	if (constructor_test == 42) terminal_write_string("Construtor executado com sucesso!\n");
	else terminal_write_string("Deu ruim cr...\n");

	terminal_printf("Teste %%: 100%%\n");
	terminal_printf("Teste %%c: %c\n", 'A');
	terminal_printf("Teste %%s: %s\n", "CavalOS");
	terminal_printf("Inteiro: %d\n", 1529);
	terminal_printf("Zero: %d\n", 0);

	terminal_write_string("\nTESTES DA LIBK\n");

	terminal_write_string("memset: ");
	for (int i = 0; i < 5; i++){
		terminal_putchar(buffer[i]);
	}

	terminal_write_string("\nstrlen: ");

	size_t tamanho = strlen("CavalOS");

	if (tamanho == 7){
		terminal_write_string("OK");
	} else {
		terminal_write_string("ERRO");
	}

	terminal_write_string("\nstrcmp: ");

	if (strcmp("CavalOS", "CavalOS") == 0){
		terminal_write_string("OK");
	} else {
		terminal_write_string("ERRO");
	}

	terminal_write_string("\nstrncmp: ");

	if (strncmp("CavalOS", "Caval", 5) == 0){
		terminal_write_string("OK");
	} else {
		terminal_write_string("ERRO");
	}

	terminal_write_string("\nstrcpy: ");

	char destino[20];
	strcpy(destino, "CavalOS");

	if (strcmp(destino, "CavalOS") == 0){
		terminal_write_string("OK");
	} else {
		terminal_write_string("ERRO");
	}
}