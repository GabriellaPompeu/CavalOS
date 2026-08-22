#include <stdint.h>

struct idt_entry{
    uint16_t offset_low; //parte baixa do end da função q trata a inter.
    uint16_t selector; //diz qual segmento do codigo deve ser utilizado quando o handler for executado.
    uint8_t zero; //é literalmente um 0
    uint8_t type_attributes; //informa as características do Gate.
    uint16_t offset_high; //parte superior do end da função q trata a inter. 
};

