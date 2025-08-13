#define UART_TX 0x10000000

void _start() {
    const char *str = "Hello, World!\n";
    for (int i = 0; str[i] != '\0'; ++i) {
        *(volatile char*)UART_TX = str[i];
    }
    
    while (1); // Infinite loop to prevent exit
}