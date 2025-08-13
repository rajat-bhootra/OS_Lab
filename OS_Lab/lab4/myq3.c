#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>

int main(){
   
    int pid = fork();

    if (pid > 0){
        printf("exiting parent %d\n",getpid());
        exit(0);
    }

    else if (pid == 0)
    {
         sleep(10);
         printf("in child %d new parent %d \n",getpid(),getppid());    
    }
    
    else{
        perror("fork");
        exit(EXIT_FAILURE);
    }
}