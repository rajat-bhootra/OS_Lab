#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void print_prime(int read_fd){
    int prime;
    if (read(read_fd,&prime,sizeof(prime)) <= 0){
        printf("final : %d \n",getpid()-1);
        exit(0);
    }

    printf("%d\n",prime);

    int pip1[2];
    pipe(pip1);

    if (fork() == 0){
        close(pip1[1]);
        print_prime(pip1[0]);
    }
    else {
        close(pip1[0]);
        int num;
        while (read(read_fd,&num,sizeof(num)) > 0)
        {
            if (num %prime != 0){
                write(pip1[1],&num,sizeof(num));
            }
        }
        close(pip1[1]);
    }
}

int main(){
    int pip2[2];
    pipe(pip2);

    if (fork() == 0){
        close(pip2[1]);
        print_prime(pip2[0]);
    }
    else{
        close(pip2[0]);
        printf("initial : %d\n",getpid());
        for (int i = 2; i < 15; i++)
        {
            write(pip2[1],&i,sizeof(i));
        }
        close(pip2[1]);
    }
}