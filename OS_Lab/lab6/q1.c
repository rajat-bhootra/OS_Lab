#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int pipeAB[2], pipeBA[2]; // Pipe for A->B and B->A
    
    if (pipe(pipeAB) == -1 || pipe(pipeBA) == -1) {
        perror("pipe");
        exit(1);
    }
    
    int pid = fork();
    
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    
    if (pid == 0) { // Child process (Process B)
        close(pipeAB[1]); // Close write end of A->B pipe
        close(pipeBA[0]); // Close read end of B->A pipe
        
        int x;
        read(pipeAB[0], &x, sizeof(x)); // Read integer from A
        printf("child %d received %d\n", getpid(), x);
        x += 10;
        write(pipeBA[1], &x, sizeof(x)); // Send modified value to A
        
        close(pipeAB[0]);
        close(pipeBA[1]);
        exit(0);
    } else { // Parent process (Process A)
        close(pipeAB[0]); // Close read end of A->B pipe
        close(pipeBA[1]); // Close write end of B->A pipe
        
        int num; // take input from user
        printf("enter a number: ");
        scanf("%d",&num);
        write(pipeAB[1], &num, sizeof(num)); // Send to B
        read(pipeBA[0], &num, sizeof(num)); // Receive from B
        printf("parent %d received %d\n", getpid(), num);
        
        close(pipeAB[1]);
        close(pipeBA[0]);
    }
    
    return 0;
}
