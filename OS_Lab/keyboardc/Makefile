run:
	riscv64-unknown-elf-gcc -mcmodel=medany -O -I. -nostdlib  -c kernel.c -o kernel.o 
	riscv64-unknown-elf-gcc -mcmodel=medany -O -I. -nostdlib  -c start.S -o start.o
	riscv64-unknown-elf-ld -T linker.ld -o kernel.elf start.o kernel.o
	riscv64-unknown-elf-objcopy -O binary kernel.elf kernel.bin
	qemu-system-riscv64 -machine virt -bios none -nographic -kernel kernel.bin
clean:
	rm -rf *.o kernel.elf kernel.bin
