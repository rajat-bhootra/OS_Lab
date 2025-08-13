#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void get_prime(int read_fd){
    int prime;
    if ((read(read_fd, &prime, sizeof(prime))) <= 0){
        printf("final process %d\n",(getpid()-1));
        exit(0);
    }

    printf("%d\n",prime);
    fflush(stdout);

    int pip[2];

    if (pipe(pip) < 0){
        perror("pip");
        exit(1);
    }
 
    if (fork() == 0){
        close(pip[1]);
        get_prime(pip[0]);
    }
    else {
        int num;
        close(pip[0]);
        while (read(read_fd, &num, sizeof(num)) > 0){
            if (num % prime != 0){
                write(pip[1],&num,sizeof(num));
            }
        }
        close(pip[1]);   
    }  
}

int main(){
    int n = 5;
    // printf("enter a num: ");
    // scanf("%d",&n);

    int pip[2];
    if (pipe(pip) < 0 ){
        perror("pip");
        exit(1);
    }
  
    int pid = fork();
    if (pid == 0){
        close(pip[1]);
        get_prime(pip[0]);
    }
    else {
        printf("initial process %d\n",getpid());
        close(pip[0]);
        for (int i = 2; i <= n; i++)
        {
            write(pip[1], &i, sizeof(i));
        }
        close(pip[1]);
    }
}