#define MTIME 0x0200BFF8UL
#define MTIMECMP 0x02004000UL
#define INTERVAL 10000000UL
#define UART 0x10000000UL
// #include <stdint.h>


void trp(){
    char * name = "Time up\n";
    while (*name){
        *(volatile char *)UART =*name;
        name++;
    }
    *(volatile unsigned long long int *)MTIMECMP =*(volatile unsigned long long int *)MTIME + INTERVAL;
}

void main(){
    
    asm volatile(
        "li t0, 0x80\n"
        "csrw mie, t0\n"
        "li t0, 0x8\n"
        "csrw mstatus, t0\n"
        :
        :
        :"t0"
    );
    while (1){
        asm volatile("wfi");
    }
    
}