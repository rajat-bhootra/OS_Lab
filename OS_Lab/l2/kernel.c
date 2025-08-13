#define UART 0x10000000UL

void pt(char * name){
    while (*name){
        *(volatile char *)UART = *name;
        name++;
    }
}

int main(){
   
    char *name = "Jai hind !!!\n";

    while (*name)
    {
        *(volatile char*)UART = *name;
        name++;
    }
    

    pt("Dosa idli sambhar chutney chutney\n");

    while (1){
        asm volatile("wfi");
    }
    return 0;
}