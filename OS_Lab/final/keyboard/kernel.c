#define UART 0x10000000UL

char getchar(){
    volatile unsigned char * uart = (volatile char*)UART;
    while (!(*(uart+5) & 0x01));
    return *uart;
}

int main(){
    while (1)
    {
        char c =getchar();
        *(volatile char*)UART = c;
    }
}