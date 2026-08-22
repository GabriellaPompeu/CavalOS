GCC = i686-elf-gcc
AS = i686-elf-as
LD = i686-elf-ld

GCCPARAMS = -m32 -ffreestanding -fno-stack-protector -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra
ASPARAMS = --32
LDPARAMS = -melf_i386

OBJECTS = loader.o kernel.o

KERNEL = mykernel.bin
ISO = myos.iso

%.o: %.c
	$(GCC) $(GCCPARAMS) -o $@ -c $<

%.o: %.s
	$(AS) $(ASPARAMS) -o $@ $<

$(KERNEL): linker.ld $(OBJECTS)
	$(LD) $(LDPARAMS) -T $< -o $@ $(OBJECTS)

iso: $(KERNEL) grub.cfg
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/
	cp grub.cfg isodir/boot/grub/
	grub-mkrescue -o $(ISO) isodir

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -f $(OBJECTS) $(KERNEL) $(ISO)
	rm -rf isodir