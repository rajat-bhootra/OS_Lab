run:
	riscv64-unknown-elf-gcc -c -O -mcmodel=medany -nostdlib kernel.c -o kernel.o
	riscv64-unknown-elf-gcc -c -O -mcmodel=medany -nostdlib start.S -o start.o
	riscv64-unknown-elf-ld -T linker.ld -o kernel.elf start.o kernel.o 
	riscv64-unknown-elf-objcopy -O binary kernel.elf kernel.bin
	qemu-system-riscv64 -machine virt -bios none -kernel kernel.bin -nographic
clean:
	rm kernel.o kernel.elf kernel.bin start.o