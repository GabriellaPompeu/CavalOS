/* Ponto de entrada de um kernel x86 de 32 bits*/

/* Meu Planejamento:
1 - Criar uma assinatura q diz ao GRUB q isto é um kernel multiboot
2 - Reservar uma área de memória para a stack
3 - Criar um _start, q será o primeiro código executado pelo kernel
4- Configurar o ESP, chamar o kernel_main em C e depois entrar em loop infinito 
*/

/*Obs.: Vou usar Assembly x86 com sintaxe AT&T*/

.set ALIGN, 1<<0 /*ALIGN = 1*/
.set MEMINFO, 1<<1 /*MEMINFO = 00000010 = 2 */
.set FLAGS, ALIGN | MEMINFO /*00000011 = 3 */
.set MAGIC, 0x1BADB002 /*padrão multiboot, ele ajuda o bootloader a encontrar o header*/
.set CHECKSUM, -(MAGIC + FLAGS) /*o multiboot exige MAGIC + FLAGS + CHECKSUM = 0, logo temos isso */

# infos para o bootloader
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# tudo sobre a memória reservada 
.section .bss
.align 16
stack_bottom:
.skip 16384 # avance 16 KiB
stack_top:
# endereços menores ficam mais perto do stack_bottom e os maiores ficam mais perto do stack_top

# intruções da CPU
.section .text
.global _start # deixei visível para o linker
.type _start, @function 
_start:
    mov $stack_top, %esp # coloquei o end de stack_top em esp
    # agr ss o processador tem uma stack válida para usar e podemos chamar kernel_main
    call kernel_main
    cli
1:  hlt # para a execução do processador até termos uma condição q o faça continuar
    jmp 1b # caso algo faça o processador retornar, voltamos para hlt

.size _start, . - _start # tamanho start = posição atual - end inicial de start