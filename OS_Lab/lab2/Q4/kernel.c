#include <stdint.h>

#define UART0      0x10000000UL      // UART base address (from device tree)
#define MTIME      0x200BFF8UL       // CLINT mtime register (for hart0 in QEMU)
#define MTIMECMP   0x2004000UL       // CLINT mtimecmp register (for hart0)
#define INTERVAL   10000000UL        // Timer interval (adjust per clock)

// Simple output: write a character to the UART.
void putchar(char c) {
    *(volatile char*)UART0 = c;
}

// Print a string.
void puts(const char *s) {
    while (*s) {
        putchar(*s++);
    }
}

// This function is called on every timer interrupt.
void timer_interrupt(void) {
    puts("hello world\n");
    // Schedule next timer interrupt.
    *(volatile uint64_t*)MTIMECMP = *(volatile uint64_t*)MTIME + INTERVAL;
}

// Kernel main: set up the timer and then wait for interrupts.
void kernel_main(void) {
    // Schedule first timer interrupt.
    *(volatile uint64_t*)MTIMECMP = *(volatile uint64_t*)MTIME + INTERVAL;

    // Enable machine timer interrupts and global interrupts.
    // (MTIE is bit 7 in mie; MIE is bit 3 in mstatus.)
    asm volatile (
        "li t0, 0x80\n"    // 0x80 = 128 (enable MTIE)
        "csrs mie, t0\n"
        "li t0, 0x8\n"     // 0x8 = 8 (enable MIE)
        "csrs mstatus, t0\n"
        :
        :
        : "t0"
    );

    // Main loop: wait for interrupt.
    while (1) {
        asm volatile ("wfi");
    }
}
