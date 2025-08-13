#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void Prime(int read_fd){

    int prime;
    if (read(read_fd, &prime ,sizeof(prime)) <= 0){
        printf("final process : %d\n",getpid()-1);
        exit(0);
    }

    printf("%d\n",prime);

    int pipe1[2];
    if (pipe(pipe1) == -1){
        perror("pipe");
        exit(1);
    }   
    
    if (fork() == 0){
        //child 
        close(pipe1[1]);
        Prime(pipe1[0]);
    }
    else{
        //parent
        close(pipe1[0]);
        int num;
        while (read(read_fd, &num, sizeof(num) > 0)){
            if (num % prime != 0){ 
               write(pipe1[1], &num, sizeof(num));
            }
        }
        close(pipe1[1]);
    }
}

int main (int argc , char * argv[]){
    if (argc != 2){
        fprintf(stderr,"Usage : %s n\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);

    int pipe2[2];
    if (pipe(pipe2) == -1 ){
        perror("pipe");
        exit(1);
    }

    if (fork() == 0){
        //child
        close(pipe2[1]);
        Prime(pipe2[0]);
    }
    else{
        printf("initial process : %d\n",getpid());
        close(pipe2[0]);
        for (int i = 2; i <= n; i++)
        {
            write(pipe2[1], &i, sizeof(i));
        }
        close(pipe2[1]);
    }
}