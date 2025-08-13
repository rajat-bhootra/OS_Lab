#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/wait.h>

void filter(int read_fd) {
    int prime;

    // Read the first number from the pipe. This is the prime for this process.
    if (read(read_fd, &prime, sizeof(prime)) <= 0){
        printf("final process : %d \n",(getpid()-1)); 
        exit(0);
    }
    
    // Print the prime found by this process.
    printf("%d\n", prime);
    fflush(stdout);
    
    int p[2];
    if (pipe(p) < 0) {
        perror("pipe");
        exit(1);
    }

    // Fork a child process to continue the filtering chain.
    if (fork() == 0) {
        // Child process: close the write end and continue filtering.
        close(p[1]);
        filter(p[0]);
        // exit(0);
    } else {
        int num;
        // Parent process: close the read end of the new pipe.
        close(p[0]);
        // Read numbers from the input pipe and pass on those not divisible by prime.
        while (read(read_fd, &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(p[1], &num, sizeof(num));
            }
        }
        close(p[1]);
        // wait(NULL);
        // exit(0);
    }
}

int main( int argc, char *argv[]) {
   
    if (argc != 2) {
        fprintf(stderr, "Usage: %s n\n", argv[0]);
        exit(1);
    }
    // int n;
    // printf("enter a num: ");
    // scanf("%d",&n);
    int n = atoi(argv[1]);
    int p[2];

    if (pipe(p) < 0) {
        perror("pipe");
        exit(1);
    }

    // Fork the first filtering process.
    if (fork() == 0) {
        // Child process: start the filter chain.
        close(p[1]);
        filter(p[0]);
        // exit(0);
    } else {
        // Parent process: generate numbers 2 through n and write them to the pipe.
        printf("initial process: %d\n",getpid());
        int num;
        close(p[0]);
        for (num = 2; num <= n; num++) {
            write(p[1], &num, sizeof(num));
        }
        close(p[1]);
        // wait(NULL);
        // exit(0);
    }
}
