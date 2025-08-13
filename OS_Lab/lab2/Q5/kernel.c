#include <stdint.h>

#define UART0_ADDR      0x10000000UL   // UART output register
#define KEYBOARD_ADDR   0x10001000UL   // Keyboard input register (hypothetical)

#define PLIC_BASE       0x0C000000UL   // PLIC base (for hart 0)
#define PLIC_PRIORITY(dev)      (*(volatile uint32_t*)(PLIC_BASE + 4*(dev)))
#define PLIC_ENABLE             (*(volatile uint32_t*)(PLIC_BASE + 0x2000))
#define PLIC_THRESHOLD          (*(volatile uint32_t*)(PLIC_BASE + 0x200000))
#define PLIC_CLAIM              (*(volatile uint32_t*)(PLIC_BASE + 0x200004))

#define KEYBOARD_IRQ_ID 10

// Simple UART output: write character to UART register.
void putchar(char c) {
    *(volatile char*)UART0_ADDR = c;
}

// Simple string output.
void puts(const char *s) {
    while (*s)
        putchar(*s++);
}

// Declare the keyboard ISR (defined in keyboard.c)
extern void keyboard_isr(void);

// Initialize the PLIC to enable keyboard interrupt.
void plic_init(void) {
    // Set keyboard interrupt priority to 1.
    PLIC_PRIORITY(KEYBOARD_IRQ_ID) = 1;
    // Enable keyboard interrupt for hart 0.
    PLIC_ENABLE |= (1 << KEYBOARD_IRQ_ID);
    // Set threshold to 0 so that interrupts with priority > 0 are delivered.
    PLIC_THRESHOLD = 0;
}

void kernel_main(void) {
    plic_init();
    puts("Kernel started. Waiting for keyboard input...\n");

    // Enable machine external interrupts (mie, bit 11) and global interrupts (mstatus, bit 3).
    asm volatile (
        "li t0, 0x800\n\t"  // 0x800 = 1 << 11 (MEIE)
        "csrs mie, t0\n\t"
        "li t0, 0x8\n\t"    // 0x8 = 1 << 3 (MIE)
        "csrs mstatus, t0\n\t"
        :
        :
        : "t0"
    );

    // Main loop: wait for interrupts.
    while (1) {
        asm volatile ("wfi");
    }
}
