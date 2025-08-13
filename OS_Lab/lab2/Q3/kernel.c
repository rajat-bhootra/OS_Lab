#define UART0 0x10000000

void putchar(char c) {
    *(volatile char *)UART0 = c;
}

void puts(const char *s) {
    while (*s) {
        putchar(*s++);
    }
}

void kernel_main() {
    puts("hello world\n");
    while (1);
}
