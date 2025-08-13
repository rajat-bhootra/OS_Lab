#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    int pid = fork();

    if (pid > 0){
        //parent 
        printf("In parent %d process, Now exiting\n",getpid());
        exit(0);
    }
    else if (pid == 0){
        //child
        sleep(5);
        printf("In child %d process newparent %d\n",getpid(),getppid());
    }
    else{
        perror("fork");
        exit(1); 
    }
}