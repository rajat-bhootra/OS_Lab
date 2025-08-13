#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
    int pid = fork();

    if (pid > 0){
        printf("parent %d sleep for 20s.\n",getpid());
        sleep(20);
        printf("exiting now!!!\n");
    }
    else if(pid == 0){
        printf("exitnig child %d\n",getpid());
        exit(0);
    }
    else{
        perror("fork");
        exit(EXIT_FAILURE);
    }
}