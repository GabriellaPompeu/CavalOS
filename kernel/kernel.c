#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "multiboot/multiboot.h"
#include "gdt.h"
#include "interrupts.h"
#include "memory/memory.h"
#include "irq.h"
#include "timer/timer.h"
#include "keyboard/keyboard.h"
#include "terminal/terminal.h"
#include "filesystem/filesystem.h"

#if defined(__linux__)
#error "Embora pareça, isso aqui n é Linux n..."
#endif

#if !defined(__i386__)
#error "Tem q compilar em ix86-elf filho..."
#endif


/* =========================
   CONSTRUTORES
   ========================= */

typedef void (*constructor_t)(void);

extern constructor_t __ctors_start[];
extern constructor_t __ctors_end[];

static void call_global_constructors(void){
    size_t count = __ctors_end - __ctors_start;

    for (size_t i = 0; i < count; i++)
        __ctors_start[i]();
}


static int constructor_test = 0;

__attribute__((constructor))
static void teste_constructor(void){
    constructor_test = 42;
}


/* =========================
   KERNEL
   ========================= */

void kernel_main(uint32_t magic, uint32_t info){
    (void)magic;

    const char* CAVALOS =
        "   _____                 _  ____   _____  \n"
        "  / ____|               | |/ __ \\ / ____| \n"
        " | |     __ ___   ____ _| | |  | | (___   \n"
        " | |    / _` \\ \\ / / _` | | |  | |\\___ \\  \n"
        " | |____ (_| |\\ V / (_| | | |__| |____) | \n"
        "  \\_____\\__,_| \\_/ \\__,_|_|\\____/|_____/\n\n";

    const char* DEVS =
        "\nDesenvolvido por Bruna Luiza, Daniel Pita,\n"
        "Felipe Dutra, Gabriella Pompeu e Raynner Meza.\n";

        /*Verifica se o GRUB realmente carregou o kernel usando multiboot 1*/
	if (!multiboot_validate(magic)){
		kernel_panic();
	}

	/*endereco da estrutura multibootinfo*/
	struct multiboot_info* mbi = (struct multiboot_info*)info;


    /* =========================
       INICIALIZAÇÃO
    ========================= */

    call_global_constructors();

    gdt_init();

    terminal_init();

    /*verifica se o GRUB forneceu o memmory map*/
	if (multiboot_has_memory_map(mbi)){
		terminal_write_string("Multiboot: mapa de memoria encontrado!\n");
	} else {
		terminal_write_string("Multiboot: mapa de memoria nao encontrado!\n");
	}

	/*mostrar regioes de memoria fornecidas pelo GRUB*/
	// multiboot_print_memory_map(mbi);

    idt_init();

    irq_init();

    keyboard_init();

    memory_map_init(info);


    /* =========================
       MENSAGEM INICIAL
       ========================= */

    terminal_write_string("==================================================\n");

    terminal_write_string("          Bem-vindo ao CavalOS!\n");

    terminal_write_string(CAVALOS);

    terminal_write_string(DEVS);

    terminal_write_string("==================================================\n");

	process_init();

    /* =========================
       TESTE DOS CONSTRUTORES
       ========================= */

    if (constructor_test == 42)
        terminal_write_string("Construtor executado com sucesso!\n");
    else
        terminal_write_string("Deu ruim cr...\n");

    terminal_input_enter();

    /* =========================
       TIMER
       ========================= */

    timer_init(100);

    /* =========================
       IRQs
       ========================= */

    irq_enable(0);  // PIT / timer
    irq_enable(1);  // teclado

    asm volatile ("sti");

    /* =========================
       LOOP PRINCIPAL
       ========================= */

    while (1){
        asm volatile ("hlt");
    }
}
