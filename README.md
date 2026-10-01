# 🐴 CavalOS

**CavalOS** é um sistema operacional experimental desenvolvido do zero com foco em aprendizado de **arquitetura de computadores, sistemas operacionais, C e Assembly**.

O projeto segue como referência a documentação e a metodologia do [OSDev Wiki](https://wiki.osdev.org/), utilizando **GRUB**, **Multiboot** e um **cross-compiler GCC para i686**.

---

## 📌 Sobre o projeto

O CavalOS está sendo desenvolvido de forma incremental, começando pela construção da infraestrutura básica do kernel e avançando gradualmente para recursos de gerenciamento de memória, interrupções e outros componentes de um sistema operacional.

O objetivo principal não é criar um sistema operacional de uso geral, mas compreender na prática como os componentes fundamentais de um SO funcionam e como eles se comunicam diretamente com o hardware.

---

## 🛠️ Tecnologias

* **C**
* **Assembly x86**
* **GCC / i686-elf-gcc**
* **GNU Make**
* **GRUB**
* **Multiboot**
* **QEMU**
* **GNU Binutils**
* **Linux**

---

## 📂 Estrutura do projeto

```text
CavalOS/
├── cavaloImg.txt
├── grub.cfg
├── isodir
│   └── boot
│       ├── grub
│       │   └── grub.cfg
│       └── mykernel.bin
├── kernel
│   ├── filesystem
│   │   ├── filesystem.c
│   │   └── filesystem.h
│   ├── gdt.c
│   ├── gdt.h
│   ├── gdt.o
│   ├── grub.cfg
│   ├── interrupts.c
│   ├── interrupts.h
│   ├── interrupts.o
│   ├── irq.c
│   ├── irq.h
│   ├── irq.o
│   ├── irq.s
│   ├── irq_stubs.o
│   ├── irq_stubs.s
│   ├── kernel.c
│   ├── kernel.o
│   ├── keyboard
│   │   ├── keyboard.c
│   │   ├── keyboard.h
│   │   └── keyboard.o
│   ├── linker.ld
│   ├── loader.o
│   ├── loader.s
│   ├── Makefile
│   ├── memory
│   │   ├── heap.c
│   │   ├── heap.h
│   │   ├── heap.o
│   │   ├── memory.c
│   │   ├── memory.h
│   │   ├── memory.o
│   │   ├── paging_asm.o
│   │   ├── paging.c
│   │   ├── paging.h
│   │   ├── paging.o
│   │   ├── pmm.c
│   │   ├── pmm.h
│   │   └── pmm.o
│   ├── multiboot
│   │   ├── multiboot.c
│   │   ├── multiboot.h
│   │   └── multiboot.o
│   ├── qemu.log
│   ├── README.md
│   ├── stack_protector.c
│   ├── stack_protector.o
│   ├── terminal
│   │   ├── terminal.c
│   │   ├── terminal.h
│   │   └── terminal.o
│   └── timer
│       ├── timer.c
│       ├── timer.h
│       └── timer.o
├── libk
│   ├── libk.a
│   ├── string.c
│   └── string.o
├── linker.ld
├── Makefile
├── mykernel.bin
├── myos.iso
├── README.md
├── sysroot
│   └── usr
│       ├── include
│       │   └── string.h
│       └── lib
│           └── libk.a
└── timer.c
```

A estrutura pode mudar conforme novos subsistemas forem adicionados ao kernel.

---

## ⚙️ Ambiente de desenvolvimento

O CavalOS utiliza um **cross-compiler** direcionado para a arquitetura `i686`.

Exemplo:

```text
i686-elf-gcc
i686-elf-as
i686-elf-ld
i686-elf-ar
```

O uso de um cross-compiler evita depender diretamente do compilador nativo do sistema operacional hospedeiro.

---

## 🔨 Compilação

Depois de configurar o ambiente e o cross-compiler, o projeto pode ser compilado utilizando:

```bash
make
```

Para remover os arquivos gerados:

```bash
make clean
```

Para reconstruir o projeto completamente:

```bash
make clean
make
```

---

## 💻 Executando o CavalOS

O kernel pode ser executado em uma máquina virtual através do **QEMU**:

```bash
make run
```

Isso permite testar o sistema operacional sem precisar instalá-lo diretamente em uma máquina física.

---

## 🧩 Componentes atuais

O projeto já possui uma base funcional de kernel com alguns componentes fundamentais:

* [x] Boot através do GRUB
* [x] Multiboot
* [x] Kernel em C
* [x] Assembly x86
* [x] Linker script
* [x] `libk`
* [x] Terminal VGA
* [x] Global constructors
* [x] GDT
* [x] IDT / estrutura inicial de interrupções
* [x] Stack Smashing Protector
* [x] Estrutura inicial de gerenciamento de memória
* [ ] Paging
* [ ] Gerenciamento completo de memória física
* [ ] Tratamento completo de interrupções
* [ ] Heap do kernel
* [ ] Drivers
* [ ] Sistema de arquivos
* [ ] Shell

> A lista representa o estado de desenvolvimento do projeto e será atualizada conforme novos componentes forem implementados.

---

## 🧠 Objetivos de aprendizado

Durante o desenvolvimento, o projeto aborda conceitos como:

* Boot e inicialização de um computador
* Funcionamento do GRUB e Multiboot
* Linkedição de um kernel
* Assembly x86
* GDT e IDT
* Interrupções
* Gerenciamento de memória
* Paging
* Stack Smashing Protector
* Comunicação com hardware
* Desenvolvimento de baixo nível
* Organização interna de sistemas operacionais

---

## 📚 Referências

O desenvolvimento do CavalOS utiliza principalmente a documentação do **OSDev Wiki** como referência:

* [OSDev Wiki](https://wiki.osdev.org/)
* [Bare Bones](https://wiki.osdev.org/Bare_Bones)
* [Meaty Skeleton](https://wiki.osdev.org/Meaty_Skeleton)

---

## 👥 Desenvolvimento

Projeto desenvolvido como parte de um estudo prático de **Sistemas Operacionais e desenvolvimento de baixo nível**.

O CavalOS é desenvolvido de maneira incremental, priorizando a compreensão dos conceitos antes da implementação de novos subsistemas.

---

## 🚧 Status

**Em desenvolvimento.**

Novos componentes estão sendo implementados gradualmente à medida que a arquitetura do kernel evolui.
