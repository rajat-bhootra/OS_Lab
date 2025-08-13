#define UART 0x10000000UL

int main(){
    char *word = "Hello\n";
    while (*word)
    {
        *(volatile char *)UART = *word;
        word++;
    }
    
    while (1)
    {
        asm volatile("wfi");
    }
    return 0;
    
}