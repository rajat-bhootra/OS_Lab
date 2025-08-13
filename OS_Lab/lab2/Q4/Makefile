# Toolchain (using your riscv64-unknown-elf- toolchain)
CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-gcc
LD = riscv64-unknown-elf-ld
OBJCOPY = riscv64-unknown-elf-objcopy
QEMU = qemu-system-riscv32

CFLAGS = -Wall -Wextra -ffreestanding -nostdlib -mabi=ilp32 -march=rv32i_zicsr
# Force 32-bit linking with correct emulation:
LDFLAGS = -Tlinker.ld -nostdlib -m elf32lriscv

all: kernel.bin

kernel.elf: start.o kernel.o trap.o
	$(LD) $(LDFLAGS) -o $@ start.o kernel.o trap.o

kernel.bin: kernel.elf
	$(OBJCOPY) -O binary $< $@

start.o: start.S
	$(AS) $(CFLAGS) -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

trap.o: trap.S
	$(AS) $(CFLAGS) -c $< -o $@

run: kernel.bin
	$(QEMU) -machine virt -bios none -kernel kernel.bin -nographic

clean:
	rm -f *.o kernel.elf kernel.bin
