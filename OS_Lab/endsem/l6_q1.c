#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    int pip1[2],pip2[2];

    pipe(pip1);
    pipe(pip2);
    
    int pid = fork();

    if (pid > 0 ){
        close(pip1[0]);
        close(pip2[1]);

        int num = 10;
        write(pip1[1],&num, sizeof(num));
        read(pip2[0],&num,sizeof(num));
        printf("parent %d recived from child %d : %d\n",getpid(),pid,num);
    }
    else if (pid ==0){
        close(pip1[1]);
        close(pip2[0]);

        int x;
        read(pip1[0],&x,sizeof(x));
        printf("child %d recived from parent %d : %d\n",getpid(),getppid(),x);
        x+=10;
        write(pip2[1],&x,sizeof(x));
    }
    else{
        perror("fork");
        exit(1);
    }
}