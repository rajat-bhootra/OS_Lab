CC=riscv64-unknown-elf-gcc
LD=riscv64-unknown-elf-ld
OBJCOPY=riscv64-unknown-elf-objcopy
QEMU=qemu-system-riscv32

CFLAGS=-Wall -Wextra -ffreestanding -nostdlib -mabi=ilp32 -march=rv32i
LDFLAGS=-T linker.ld -nostdlib -m elf32lriscv  # Force 32-bit linking

all: kernel.elf kernel.bin

kernel.elf: start.o kernel.o
	$(LD) $(LDFLAGS) -o $@ $^

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

start.o: start.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

run: kernel.bin
	$(QEMU) -machine virt -bios none -kernel kernel.bin -nographic

clean:
	rm -f *.o kernel.elf kernel.bin