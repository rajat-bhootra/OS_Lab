#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int pid = fork();

    if (pid > 0){
        // parent 
        printf("in parent %d, sleeping for 30s\n",getpid());
        sleep(30);
        printf("child killed\n");
        exit(0);
    }
    else if (pid == 0){
        // child
        printf("exiting child %d immediately\n",getpid());
        exit(0);
    }
    else{
        perror("fork");
        exit(1);
    }
}