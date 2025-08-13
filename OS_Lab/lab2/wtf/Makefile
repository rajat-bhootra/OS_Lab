run:
	# riscv64-unknown-elf-gcc -O -mcmodel=medany -nostdlib  -c start.S -o start.o
	# riscv64-unknown-elf-gcc -O -mcmodel=medany -nostdlib  -c trap.S -o trap.o
	riscv64-unknown-elf-gcc -O -mcmodel=medany -nostdlib  -c kernel.c -o kernel.o
	# riscv64-unknown-elf-ld -T linker.ld -o kernel.elf start.o kernel.o
	riscv64-unknown-elf-ld -T linker.ld -o kernel.elf kernel.o
	riscv64-unknown-elf-objcopy -O binary kernel.elf kernel.bin
	qemu-system-riscv64 -bios none -machine virt -kernel kernel.bin -nographic
clean:
	# rm kernel.o start.o trap.o kernel.bin kernel.elf
	rm kernel.o  kernel.bin kernel.elf
