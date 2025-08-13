#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(){
 
    int pid = fork();

    if (pid > 0){
        //parent 
        printf("in parent %d , now exiting\n",getpid());
        exit(0);
    }
    else if (pid == 0){
        //child
        // wait for parent termination.
        sleep(2);
        printf("in child %d, new parent %d\n",getpid(),getppid());
    }
    else{
        perror("fork");
        exit(1);
    }

}