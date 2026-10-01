GCC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-ld
AR = i686-elf-ar

GCCPARAMS = -m32 -ffreestanding -fstack-protector -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra --sysroot=$(SYSROOT) -I$(SYSROOT)/usr/include
ASPARAMS = --32
LDPARAMS = -melf_i386

KERNEL_OBJECTS = kernel/loader.o kernel/kernel.o kernel/multiboot/multiboot.o kernel/gdt.o kernel/interrupts.o kernel/memory/pmm.o kernel/irq.o kernel/irq_stubs.o kernel/timer/timer.o kernel/keyboard/keyboard.o kernel/terminal/terminal.o kernel/memory/memory.o kernel/stack_protector.o kernel/memory/paging.o kernel/memory/heap.o
LIBK = libk/libk.a
LIBK_PATH = $(SYSROOT)/usr/lib
LIBK_OBJECTS = libk/string.o

KERNEL = mykernel.bin
ISO = myos.iso

SYSROOT = $(PWD)/sysroot

%.o: %.c
	$(GCC) $(GCCPARAMS) -o $@ -c $<

%.o: %.s
	$(AS) $(ASPARAMS) -o $@ $<

libk/libk.a: libk/string.o 
	$(AR) rcs $@ $<

$(KERNEL): linker.ld $(KERNEL_OBJECTS) $(SYSROOT)/usr/lib/libk.a
	$(LD) $(LDPARAMS) -T $< -L$(LIBK_PATH) -o $@ $(KERNEL_OBJECTS) -lk

$(SYSROOT)/usr/lib/libk.a: libk/libk.a
	mkdir -p $(SYSROOT)/usr/lib
	cp libk/libk.a $@

iso: $(KERNEL) grub.cfg
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(ISO) isodir

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -f $(KERNEL_OBJECTS) $(KERNEL) $(ISO) $(LIBK) $(LIBK_OBJECTS)
	rm -rf isodir

install: libk/libk.a
	mkdir -p $(SYSROOT)/usr/lib
	cp libk/libk.a $(SYSROOT)/usr/lib
