#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int pid = fork();

    if (pid > 0) {  
        // Parent process: exit immediately to orphan the child.
        printf("Parent process (PID: %d) exiting immediately...\n", getpid());
        exit(0);
    } else if (pid == 0) {  
        // Child process: becomes orphan after the parent exits.
        // Wait a moment to ensure the parent has terminated.
        sleep(2);
        printf("Child process (PID: %d) now orphaned, new parent PID: %d\n", getpid(), getppid());
        
        // Continue running. This loop allows you to observe that the process is still active.
        // while(1) {
		// 	sleep(1);
        // }
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}


// // A C program to demonstrate Orphan Process. 
// // Parent process finishes execution while the 
// // child process is running. The child process 
// // becomes orphan. 
// #include<stdio.h> 
// #include <sys/types.h> 
// #include <unistd.h> 

// int main() 
// { 
// 	// Create a child process	 
// 	int pid = fork(); 

// 	if (pid > 0) 
// 		printf("in parent process\n"); 

// 	// Note that pid is 0 in child process 
// 	// and negative if fork() fails 
// 	else if (pid == 0) 
// 	{ 
// 		sleep(30); 
// 		printf("in child process\n"); 
// 	} 

// 	return 0; 
// } 
