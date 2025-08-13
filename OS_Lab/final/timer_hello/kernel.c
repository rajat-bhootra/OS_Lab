#define MTIME 0x0200BFF8UL
#define MTIMECMP 0x02004000UL
#define INTERVAL 30000000UL
#define UART 0x10000000UL

void trap(){
     char *str = "TIME UP!!!\n";
     while (*str)
     {
        *(volatile char*)UART = *str;
        str++;
     }
     *(volatile unsigned long long int* )MTIMECMP = *(volatile unsigned long long int* )MTIME + INTERVAL;
}

int main(){
    asm volatile(
        "li t0, 0x80\n" //MIE 
        "csrw mie, t0\n"
        "li t0,0x8\n" //MSTATUS
        "csrw mstatus, t0\n"
        :
        :
        :"t0"
    );

    while (1)
    {
        asm volatile("wfi");
    }
    
}