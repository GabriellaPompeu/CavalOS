#include <stdbool.h>
#include <stddef.h>
#include <stdint.h> 
#include "interrupts.h"

struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idtp;

void idt_set_gate(uint8_t index, uint32_t handler, uint16_t selector, uint8_t type_attributes){ // dá os valores para a idt
    idt[index].offset_low = handler & 0xFFFF;
    idt[index].selector = selector;
    idt[index].zero = 0;
    idt[index].type_attributes = type_attributes;
    idt[index].offset_high = (handler >> 16) & 0xFFFF;
}

void idt_init(void){ //inicializa a idt zerando todos os atributos e define o endereço que inicia e o maior byte
    for (size_t i = 0; i < IDT_ENTRIES; i++){
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attributes = 0;
        idt[i].offset_high = 0;
    }

    idtp.limit = sizeof(idt) - 1;
    idtp.base = (uint32_t)&idt;
}

static const char* exception_messages[32] = {
    "Divisao por zero", "Debug",
    "Interrupcao nao mascaravel", "Breakpoint",
    "Overflow", "Limite excedido",
    "Opcode invalido", "Dispositivo indisponivel",
    "Double fault", "Coprocessor Segment Overrun",
    "TSS invalido", "Segmento nao presente",
    "Falha de segmento de pilha", "Falha de protecao geral",
    "Falha de pagina", "Vaga",
    "Ponto flutuante x87", "Alignment Check",
    "Machine Check", "SIMD ponto flutuante",
    "Virtualizacao", "Protecao controle",
    "Vaga", "Vaga",
    "Vaga", "Vaga",
    "Vaga", "Vaga",
    "Hypervisor Injection", "Comunicacao VMM",
    "Seguranca", "Vaga"
};

void exception_handler(struct registers *r){
    if (r->int_no < 32) {
        // ainda vou criar a biblioteca stdio, então vamos poder por exemplo usar o printf para printas as mensagens
    }
}
