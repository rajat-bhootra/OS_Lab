#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void sieve(int read_fd){
    int prime;
    if ( read(read_fd, &prime, sizeof(prime))<= 0){
         printf("final process %d\n",(getpid()-1));
         exit(0);
    }

    printf("%d\n",prime);
    fflush(stdout);

    int p[2];
    if (pipe(p)<0){
        perror("pipe");
        exit(1);
    }

    if (fork()==0){
        //child
        close(p[1]);
        sieve(p[0]);
    }
    else{
        close(p[0]);
        int num;
        while (read(read_fd,&num, sizeof(num)) > 0){
              if (num % prime != 0){
                 write(p[1],&num,sizeof(num));
              }
        }
        close(p[1]);
    }
}

int main(int argc ,char *argv[]){
    if (argc != 2){
        printf("Usage : %s n\n",argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);
   
    int p[2];

    if (pipe(p) < 0){
        perror("pipe");
        exit(1);
    }

    if (fork() == 0){
        close(p[1]);
        sieve(p[0]);
    }
    else{
        printf("initial process %d",getpid());
        close(p[0]);
        for (int i = 2; i <=n; i++)
        {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
        
    }

    
}