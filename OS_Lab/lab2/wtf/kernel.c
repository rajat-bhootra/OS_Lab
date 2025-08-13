#define UART 0x10000000

void mainv(){
    char *name = "bhupendra jogi\n";
    while (*name){
        *(volatile char *)UART = *name;
        name++;
    }
    while (1);
}