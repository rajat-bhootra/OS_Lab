#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){

    //create two pipes.
    int pipeAB[2],pipeBA[2];
     
    if (pipe(pipeAB) == -1 || pipe(pipeBA) == -1 ){
        perror("pipe");
        exit(1);
    }

    //fork process
    int pid = fork();

    if (pid > 0){
        //parent
        close(pipeAB[0]);
        close(pipeBA[1]);
        
        int num;
        printf("enter a number: ");
        scanf("%d",&num);
        write(pipeAB[1],&num, sizeof(num));
        read(pipeBA[0],&num,sizeof(num));
        printf("parent(pid %d) recieved from child(pid %d) : %d\n",getpid(),pid,num);

    } 
    else if (pid == 0)
    {
        // child
        close(pipeAB[1]);
        close(pipeBA[0]);
        
        int x; //temp variable
        read(pipeAB[0],&x,sizeof(x));
        printf("child(pid %d) recieved from parent(pid %d) : %d\n",getpid(),getppid(),x);
        x+=10; //increment;
        write(pipeBA[1],&x, sizeof(x));
    }
    else{
        perror("fork");
        exit(1);
    }
}