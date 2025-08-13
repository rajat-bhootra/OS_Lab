#include <stdint.h>

// Forward declaration of putchar from kernel.c
extern void putchar(char c);

// This function is called from the trap handler when a keyboard IRQ is claimed.
void keyboard_isr(void) {
    // Read the key from the keyboard data register and echo it.
    char key = *(volatile char*)0x10001000;  // KEYBOARD_ADDR
    putchar(key);
}
