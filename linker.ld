ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

SECTIONS
{
    . = 2M;
    .text BLOCK(4K) : ALIGN(4K){
        *(.multiboot)
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K){
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K){
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K){
        *(COMMON)
        *(.bss)
    }

    .init_array ALIGN(CONSTANT (MAXPAGESIZE)) : {
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);
    }
}
