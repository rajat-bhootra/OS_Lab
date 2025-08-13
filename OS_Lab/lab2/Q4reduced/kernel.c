#include <stdint.h>

#define UART0      0x10000000UL      // UART base address (from device tree)
#define MTIME      0x0200BFF8UL       // CLINT mtime register
#define MTIMECMP   0x02004000UL       // CLINT mtimecmp register
#define INTERVAL   10000000UL        // Timer interval (adjust per clock)

// Memory-mapped I/O for UART
#define UART0_THR (volatile uint8_t*)(UART0 + 0x00) // THR: transmitter holding register
#define UART_LSR_EMPTY_MASK 0x40                    // LSR Bit 6: Transmitter empty

// Write a character to UART
void uart_putc(char c) {
    // Wait until UART is ready to transmit
    // while((*UART0_LSR & UART_LSR_EMPTY_MASK) == 0);
    *UART0_THR = c;
}

// Write a string to UART
void uart_puts(const char *s) {
    while(*s) {
        uart_putc(*s++);
    }
}

// This function is called on every timer interrupt.
void timer_interrupt(void) {
    uart_puts("hello world\n");
    // Schedule next timer interrupt.
    *(volatile uint64_t*)MTIMECMP = *(volatile uint64_t*)MTIME + INTERVAL;
}

// Kernel main: set up the timer and then wait for interrupts.
void kernel_main(void) {
    uart_puts("Initializing kernel...\n");

    // Schedule first timer interrupt.
    uint64_t current_time = *(volatile uint64_t*)MTIME;
    *(volatile uint64_t*)MTIMECMP = current_time + INTERVAL;
    
    uart_puts("Timer configured...\n");

    // Enable machine timer interrupts and global interrupts.
    asm volatile (
        "li t0, 0x80\n\t"      // MTIE (bit 7)
        "csrs mie, t0\n\t"
        "li t0, 0x8\n\t"       // MIE (bit 3)
        "csrs mstatus, t0\n\t"
        :
        :   
        : "t0"
    );

    uart_puts("Interrupts enabled...\n");
    uart_puts("Entering main loop...\n");

    // Main loop: wait for interrupt.
    while (1) {
        asm volatile ("wfi");
    }
}

// Change from trap_handler to trap_handler_c
void trap_handler_c(void) {
    // Read mcause to determine interrupt cause
    uint64_t mcause;
    asm volatile("csrr %0, mcause" : "=r" (mcause));
    
    // Check if it's a timer interrupt (machine timer interrupt has cause 0x8000000000000007)
    if (mcause == 0x8000000000000007) {
        timer_interrupt();
    }
    
    // Clear the interrupt
    asm volatile("csrw mip, zero");
}
