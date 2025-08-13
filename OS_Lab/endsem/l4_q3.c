#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){

    int pid = fork();

    if (pid  > 0){
        //parent
        printf("parent %d , sleeping for 10s\n",getpid());
        sleep(10);
        printf("child killed\n");
        exit(0);
    }
    else if (pid == 0){
        printf("child %d , exiting\n",getpid());
        exit(0);
    }
    else {
        perror("fork");
        exit (1);
    }
}