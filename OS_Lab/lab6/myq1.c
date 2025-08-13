#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int pipeAB[2],pipeBA[2];

    if (pipe(pipeAB) == -1 || pipe(pipeBA) ==- 1){
        perror("pipe");
        exit(1);
    }

    int pid = fork();

    if (pid > 0){
        close(pipeAB[0]);
        close(pipeBA[1]);

        int num;
        printf("enter a number: ");
        scanf("%d",&num);
        write(pipeAB[1], &num, sizeof(num));
        read(pipeBA[0], &num, sizeof(num));
        printf("parent %d received %d \n",getpid(), num);

        // close(pipeAB[1]);
        // close(pipeBA[0]);
    }
    else if (pid == 0){
        close(pipeAB[1]);
        close(pipeBA[0]);

        int x;
        read(pipeAB[0], &x, sizeof(x));
        printf("child %d received %d\n", getpid(), x);
        x+=10;
        write(pipeBA[1], &x, sizeof(x));

        // close(pipeAB[0]);
        // close(pipeBA[1]);
    }
    else{
        perror("fork");
        exit(1);
    }
     
    return 0;
}
