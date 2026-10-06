# CavalOS --- Documentação dos Módulos

## 1. Visão geral

O **CavalOS** é um sistema operacional de 32 bits desenvolvido em C e
Assembly para a arquitetura x86/i386. O projeto segue a abordagem do
**OSDev**, começando pelo Bare Bones e evoluindo para uma estrutura
semelhante a um Meaty Skeleton.

O objetivo do trabalho é construir, de forma incremental, os principais
componentes de um sistema operacional: inicialização do kernel,
terminal, interrupções, gerenciamento de memória, processos, sistema de
arquivos, syscalls e, posteriormente, um shell.

### Principais decisões do projeto

-   Arquitetura alvo: **i386 / x86 de 32 bits**.
-   Kernel compilado com **cross compiler `i686-elf-gcc`**.
-   Linguagens principais: **C e Assembly x86**.
-   Bootloader: **GRUB**, utilizando o protocolo Multiboot.
-   Execução/testes: **QEMU**.
-   Terminal inicial: **VGA Text Mode**, utilizando a memória `0xB8000`.
-   O projeto é desenvolvido de forma incremental, evitando implementar
    mecanismos complexos antes que suas dependências estejam prontas.
-   Alguns módulos foram desenvolvidos por colaboradores do projeto.
    Quando isso ocorre, o código do colaborador é considerado a
    implementação de referência daquele módulo.

------------------------------------------------------------------------

# 2. Estrutura inicial --- Bare Bones / Meaty Skeleton

O projeto começou seguindo a estrutura tradicional do OSDev:

``` text
GRUB
 ↓
loader.s
 ↓
kernel_main()
 ↓
inicialização dos subsistemas
 ↓
loop principal do kernel
```

O objetivo dessa etapa foi criar um kernel mínimo capaz de ser
compilado, transformado em ISO e iniciado pelo GRUB dentro do QEMU.

A estrutura foi posteriormente expandida para separar os módulos do
sistema operacional em diretórios próprios.

------------------------------------------------------------------------

# 3. Loader / Boot

## Objetivo

O `loader.s` é o primeiro código do CavalOS executado após o GRUB
entregar o controle ao kernel.

Ele é responsável principalmente por:

-   declarar o cabeçalho Multiboot;
-   reservar a stack inicial do kernel;
-   configurar `ESP`;
-   receber os valores fornecidos pelo GRUB;
-   chamar `kernel_main`.

## Implementação

A stack é reservada estaticamente:

``` asm
stack_bottom:
.skip 16384
stack_top:
```

e o ponteiro de stack é configurado com:

``` asm
mov $stack_top, %esp
```

O GRUB fornece:

-   `EAX` → magic number do Multiboot;
-   `EBX` → endereço das informações do Multiboot.

Esses valores são passados para:

``` c
kernel_main(uint32_t magic, uint32_t info)
```

O loader também contém o loop final do kernel:

``` asm
cli
1:
    hlt
    jmp 1b
```

------------------------------------------------------------------------

# 4. Global Constructors

## Objetivo

Permitir que funções marcadas com:

``` c
__attribute__((constructor))
```

sejam executadas durante a inicialização do kernel.

## Consideração

Como o CavalOS é um kernel freestanding, não existe o ambiente de
runtime tradicional de um programa comum. Portanto, os construtores não
são executados automaticamente como em um programa de usuário.

## Implementação

O linker script fornece as regiões:

``` text
__ctors_start
__ctors_end
```

O kernel percorre essa região e chama cada função:

``` c
static void call_global_constructors(void)
```

Foi utilizado um teste com uma variável inicializada pelo construtor
para verificar se o mecanismo realmente funciona.

------------------------------------------------------------------------

# 5. Terminal

## Objetivo

Fornecer uma forma básica de comunicação entre o kernel e o usuário.

## Implementação

O terminal utiliza o modo texto VGA, cuja memória começa em:

``` text
0xB8000
```

O modo utilizado possui:

``` text
80 colunas × 25 linhas
```

Cada posição da tela ocupa dois bytes:

``` text
caractere + atributo de cor
```

O módulo possui funções para:

-   imprimir caracteres;
-   imprimir strings;
-   formatar valores;
-   controlar posição do cursor;
-   apagar/backspace;
-   entrada de teclado;
-   histórico de comandos.

Também existe suporte a uma função semelhante ao `printf`:

``` c
terminal_printf(...)
```

Essa função é utilizada por diversos módulos para apresentar informações
e mensagens de diagnóstico.

------------------------------------------------------------------------

# 6. Stack Smashing Protector --- SSP

## Objetivo

Adicionar uma proteção básica contra corrupção da stack.

O compilador é executado com:

``` text
-fstack-protector
```

O kernel fornece:

``` c
__stack_chk_guard
__stack_chk_fail()
```

O valor utilizado inicialmente para o guard é:

``` text
0xDEADBEEF
```

Se o compilador detectar alteração indevida do valor de proteção,
`__stack_chk_fail()` informa o problema e provoca um kernel panic.

## Consideração

Foi escolhido não realizar uma corrupção forçada da stack apenas para
demonstrar o mecanismo. A implementação foi mantida como uma proteção
real do kernel.

------------------------------------------------------------------------

# 7. Multiboot

## Objetivo

Permitir que o GRUB carregue o CavalOS e forneça informações sobre o
ambiente de inicialização.

## Implementação

O loader possui o cabeçalho Multiboot com:

``` text
MAGIC
FLAGS
CHECKSUM
```

Durante a inicialização, o kernel verifica o magic number recebido.

Também é utilizado o `multiboot_info`, fornecido pelo GRUB, para
consultar informações como o mapa de memória.

O projeto possui funções como:

``` c
multiboot_validate()
multiboot_has_memory_map()
multiboot_print_memory_map()
```

------------------------------------------------------------------------

# 8. GDT --- Global Descriptor Table

## Objetivo

Configurar os segmentos utilizados pelo processador em modo protegido.

A GDT possui seis entradas:

    Entrada   Seletor Função
  --------- --------- -------------
          0    `0x00` Null
          1    `0x08` Kernel Code
          2    `0x10` Kernel Data
          3    `0x18` User Code
          4    `0x20` User Data
          5    `0x28` TSS

Cada descriptor ocupa 8 bytes.

## Implementação

A parte em C monta as entradas da GDT. A parte em Assembly executa:

``` asm
lgdt gp
```

Depois os registradores de segmento de dados são configurados com:

``` text
0x10
```

e `CS` é atualizado através de um **far jump** para:

``` text
0x08
```

O far jump é necessário porque `CS` não pode ser alterado simplesmente
com `mov`.

## TSS

Uma entrada de TSS já foi criada e configurada na GDT. Entretanto, o
carregamento do TSS com `LTR` ainda faz parte da evolução do suporte a
processos/modo usuário.

------------------------------------------------------------------------

# 9. IDT --- Interrupt Descriptor Table

## Objetivo

Definir para o processador quais funções devem ser executadas quando uma
interrupção ou exceção ocorrer.

A IDT possui:

``` text
256 entradas
```

e cada entrada contém o endereço do handler, seletor de segmento e
atributos.

O IDT é carregado através de:

``` asm
lidt
```

## Interrupções atualmente configuradas

Inicialmente foram configuradas:

``` text
INT 0x20 → IRQ0 → Timer
INT 0x21 → IRQ1 → Teclado
```

Os handlers utilizam o segmento de código do kernel:

``` text
0x08
```

e o atributo:

``` text
0x8E
```

para interrupções de hardware em nível de kernel.

------------------------------------------------------------------------

# 10. PIC --- Programmable Interrupt Controller

## Objetivo

Configurar o controlador de interrupções para que as IRQs de hardware
não entrem em conflito com as exceções da CPU.

## Implementação

O PIC é remapeado para:

``` text
PIC1 → 0x20
PIC2 → 0x28
```

Consequentemente:

``` text
IRQ0 → INT 0x20
IRQ1 → INT 0x21
...
IRQ8 → INT 0x28
```

Inicialmente todas as IRQs são mascaradas.

Depois o kernel libera especificamente:

``` text
IRQ0 → Timer
IRQ1 → Teclado
```

Ao terminar uma IRQ, o PIC recebe o comando EOI:

``` text
0x20
```

Quando a interrupção pertence ao PIC2, o EOI também é enviado ao PIC1.

------------------------------------------------------------------------

# 11. IRQ Stubs

## Objetivo

Criar a ponte em Assembly entre o hardware e o código C.

Para o timer:

``` asm
irq0:
    pusha
    pushl $32
    call irq_handler
    addl $4, %esp
    popa
    iret
```

Para o teclado:

``` asm
irq1:
    pusha
    pushl $33
    call irq_handler
    addl $4, %esp
    popa
    iret
```

O `pusha` preserva os registradores gerais antes de entrar no código C.

Depois do handler, `popa` restaura os registradores e `iret` retorna da
interrupção.

------------------------------------------------------------------------

# 12. Timer / PIT

## Objetivo

Criar uma fonte periódica de interrupções para o kernel.

## Implementação

O timer utiliza o **PIT --- Programmable Interval Timer**.

A frequência é configurada através do divisor:

``` text
1193182 / frequência desejada
```

No estado atual, o kernel chama:

``` c
timer_init(100);
```

resultando em aproximadamente 100 interrupções por segundo.

O contador:

``` c
timer_ticks
```

é incrementado a cada interrupção.

A cada 10 ticks, o timer chama:

``` c
scheduler_run_next();
```

Esse módulo foi desenvolvido por um colaborador do projeto e é tratado
como código de referência do colaborador.

------------------------------------------------------------------------

# 13. Teclado

## Objetivo

Receber entrada do teclado através da IRQ1.

O fluxo é:

``` text
Teclado
 ↓
IRQ1
 ↓
INT 0x21
 ↓
irq1
 ↓
irq_handler(33)
 ↓
keyboard_handler()
 ↓
terminal
```

O módulo traduz a entrada do teclado e permite que o terminal receba
caracteres.

O código do teclado também foi desenvolvido por um colaborador do
projeto.

------------------------------------------------------------------------

# 14. Exceções e Kernel Panic

## Objetivo

Detectar situações de erro do processador e interromper o kernel de
maneira controlada.

As primeiras 32 entradas são reservadas para exceções da CPU.

Foi criada uma tabela com mensagens para exceções como:

``` text
Divisão por zero
Opcode inválido
Double fault
Falha de proteção geral
Falha de página
...
```

Quando uma exceção tratada chega ao `exception_handler()`, o kernel
exibe a mensagem e chama:

``` c
kernel_panic();
```

O `kernel_panic()` desabilita interrupções e mantém o processador
parado.

------------------------------------------------------------------------

# 15. Gerenciamento de Memória Física --- PMM

## Objetivo

Controlar quais frames de memória física estão livres ou ocupados.

## Implementação

O PMM utiliza um bitmap para representar os frames.

Cada frame pode ser marcado como:

``` text
livre
ocupado
```

Entre as operações disponíveis estão:

``` c
pmm_init()
pmm_free_region()
pmm_reserve_region()
alloc_frame()
free_frame()
```

Também existem funções para obter:

``` text
endereço do bitmap
tamanho do bitmap
total de frames
frames usados
frames livres
```

A memória disponível é obtida através do mapa fornecido pelo Multiboot.

## Consideração

O código original desenvolvido pelo colaborador é considerado a
referência deste módulo. Alterações externas não devem substituir ou
descaracterizar essa implementação.

------------------------------------------------------------------------

# 16. Paginação

## Objetivo

Preparar o sistema para utilizar memória virtual.

O módulo possui estruturas e operações para:

-   criar page tables;
-   mapear páginas;
-   remover mapeamentos;
-   consultar páginas;
-   traduzir endereços;
-   verificar se uma página está mapeada.

Entre as funções existentes:

``` c
pagina_init()
map_page()
unmap_page()
create_page_table()
add_page_table()
get_page()
is_page_mapped()
translate_address()
```

Existe também uma rotina Assembly para habilitar o bit de paginação do
`CR0`.

## Consideração

A paginação depende da correta preparação das estruturas de memória
física e ainda faz parte da evolução do gerenciamento de memória do
kernel.

------------------------------------------------------------------------

# 17. Heap do Kernel

## Objetivo

Fornecer alocação dinâmica de memória para o kernel.

Foram implementadas funções semelhantes às utilizadas em programas
convencionais:

``` c
kmalloc()
kfree()
```

O heap depende da infraestrutura de memória do kernel.

A ideia é permitir que estruturas cujo tamanho ou quantidade não seja
conhecida durante a compilação possam ser criadas dinamicamente.

------------------------------------------------------------------------

# 18. Filesystem

## Objetivo

Criar uma primeira camada de armazenamento de arquivos para o CavalOS.

## Consideração principal

Nesta etapa, o filesystem é **inteiramente em memória RAM**.

Não existe ainda suporte a:

-   disco;
-   FAT;
-   ext;
-   outro filesystem persistente.

Isso foi uma escolha deliberada para permitir desenvolver a interface de
arquivos antes de lidar com drivers de armazenamento.

## Operações

O módulo permite:

``` c
filesystem_init()
filesystem_find_file()
filesystem_create_file()
filesystem_write_file()
filesystem_read_file()
filesystem_delete_file()
filesystem_list_files()
```

Cada arquivo possui:

-   nome;
-   tamanho;
-   dados;
-   estado de utilização.

Existe também um limite máximo de arquivos e de tamanho por arquivo.

------------------------------------------------------------------------

# 19. Processos

## Objetivo

Criar a base para execução e gerenciamento de múltiplos processos.

O módulo possui operações como:

``` c
process_init()
process_create()
process_find()
process_list_all()
process_get_current()
process_get_next_ready()
process_set_current()
process_destroy()
process_set_state()
```

Também existe uma estrutura de estados de processo, incluindo o estado:

``` text
PROCESS_TERMINATED
```

O scheduler possui funções como:

``` c
scheduler_next()
scheduler_run_next()
```

## Estado atual

A infraestrutura básica de processos e scheduler já existe, mas a troca
de contexto completa e o suporte a processos em modo usuário ainda
dependem das etapas de Assembly, memória e proteção de níveis de
privilégio.

------------------------------------------------------------------------

# 20. Syscalls

## Objetivo

Criar uma interface para que programas possam solicitar serviços do
kernel.

## Syscalls disponíveis

Atualmente existem:

  Syscall        Função
  -------------- ----------------------
  `SYS_WRITE`    Escrever no terminal
  `SYS_PS`       Listar processos
  `SYS_LS`       Listar arquivos
  `SYS_GETPID`   Obter PID atual
  `SYS_EXIT`     Encerrar o processo

O dispatcher possui a estrutura:

``` c
syscall_dispatch(number, arg1, arg2, arg3)
```

e encaminha cada número para o serviço correspondente.

## Estado atual

A função:

``` c
syscall()
```

atualmente chama diretamente:

``` text
syscall()
 ↓
syscall_dispatch()
```

Portanto, ainda não existe uma transição real entre modo usuário e
kernel através de uma instrução de syscall.

A próxima evolução será implementar o mecanismo de entrada por
interrupção, tradicionalmente usando:

``` text
INT 0x80
```

e posteriormente integrar isso com os processos em modo usuário.

------------------------------------------------------------------------

# 21. Assembly do sistema

O Assembly é utilizado somente onde o acesso direto ao processador é
necessário ou conveniente.

Entre as rotinas Assembly existentes ou planejadas estão:

-   loader;
-   GDT;
-   IRQ stubs;
-   habilitação da paginação;
-   troca de contexto;
-   entrada/saída de syscalls.

## Próximas etapas de Assembly

Ainda estão previstas implementações mais completas para:

``` text
GDT
Interrupções
PIC
Gerenciamento de memória
Processos
Syscalls
```

A intenção é manter C como linguagem principal do kernel e utilizar
Assembly para as operações específicas da arquitetura x86.

------------------------------------------------------------------------

# 22. Shell

## Objetivo final desta etapa

Criar uma interface de comandos para o usuário interagir com o CavalOS.

O shell deverá utilizar os componentes já construídos, principalmente:

``` text
Terminal
   ↓
Filesystem
   ↓
Processos
   ↓
Syscalls
```

Exemplos de comandos planejados:

``` text
ls
ps
echo
clear
...
```

O shell será implementado depois que os mecanismos necessários de
processos, syscalls e entrada de usuário estiverem suficientemente
estáveis.

------------------------------------------------------------------------

# 23. Fluxo atual do kernel

A inicialização atual segue aproximadamente esta ordem:

``` text
GRUB
 ↓
loader.s
 ↓
kernel_main()
 ↓
Validação Multiboot
 ↓
Construtores globais
 ↓
GDT
 ↓
Terminal
 ↓
Informações Multiboot
 ↓
IDT
 ↓
PIC
 ↓
Teclado
 ↓
Gerenciamento de memória
 ↓
Filesystem
 ↓
Processos
 ↓
Syscalls
 ↓
Timer
 ↓
Habilitação das IRQs
 ↓
STI
 ↓
Loop com HLT
```

Depois de `sti`, o kernel permanece aguardando interrupções:

``` text
hlt
 ↓
interrupção
 ↓
handler
 ↓
retorno ao kernel
 ↓
hlt
```

------------------------------------------------------------------------

# 24. Estado geral do projeto

## Concluído ou funcional em nível inicial

-   [x] Boot pelo GRUB
-   [x] Bare Bones / Meaty Skeleton
-   [x] Global Constructors
-   [x] Terminal VGA
-   [x] Formatação de saída
-   [x] Stack Smashing Protector
-   [x] Multiboot
-   [x] GDT básica
-   [x] IDT básica
-   [x] PIC
-   [x] IRQs de timer e teclado
-   [x] Timer
-   [x] Teclado
-   [x] Gerenciamento básico de memória física
-   [x] Estrutura de paginação
-   [x] Heap do kernel
-   [x] Filesystem em RAM
-   [x] Estrutura básica de processos
-   [x] Estrutura inicial de syscalls

## Em desenvolvimento / próximas etapas

-   [ ] Assembly completo das interrupções
-   [ ] TSS e troca de contexto
-   [ ] Gerenciamento de memória virtual completo
-   [ ] Processos em modo usuário
-   [ ] Syscalls reais via entrada de hardware (`INT 0x80` ou mecanismo
    equivalente)
-   [ ] Mais funções para programas de usuário
-   [ ] Shell

------------------------------------------------------------------------

# 25. Considerações finais

O CavalOS foi desenvolvido de forma incremental. A principal preocupação
do projeto é construir primeiro as bases necessárias para que
componentes mais avançados possam ser implementados de maneira segura.

A divisão em módulos permite que cada parte do sistema seja desenvolvida
e testada separadamente:

``` text
Boot
 ↓
CPU / GDT / IDT
 ↓
Interrupções
 ↓
Memória
 ↓
Processos
 ↓
Syscalls
 ↓
Programas de usuário
 ↓
Shell
```

Essa organização também facilita a identificação de erros, pois cada
nova camada depende de componentes anteriores já estabelecidos.

O estado atual representa um kernel funcional em evolução, ainda
distante de um sistema operacional completo, mas já contendo as
principais estruturas necessárias para avançar para execução de
programas de usuário e uma interface de shell.
