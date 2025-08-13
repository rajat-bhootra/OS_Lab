#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
// #include <sys/wait.h>

int main() {
    int pid = fork();

    if (pid > 0) {  // Parent process
        printf("Parent (%d): child process created. Sleeping for 20 seconds...\n", getpid());
        // Sleep long enough so you can inspect the zombie process (e.g., using 'ps' or 'top')
        sleep(20);
        // After sleep, reap the child process to clean up the zombie
        // wait(NULL);
        printf("Parent: child reaped, exiting now.\n");
    } else if (pid == 0) {  // Child process
        printf("Child (%d): exiting immediately to become a zombie.\n", getpid());
        exit(0);
    } else {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}


// // A C program to demonstrate Zombie Process. 
// // Child becomes Zombie as parent is sleeping 
// // when child process exits. 
// #include <stdlib.h> 
// #include <sys/types.h> 
// #include <unistd.h> 
// int main() 
// { 
// 	// Fork returns process id 
// 	// in parent process 
// 	pid_t child_pid = fork(); 

// 	// Parent process 
// 	if (child_pid > 0) 
// 		sleep(50); 

// 	// Child process 
// 	else		
// 		exit(0); 

// 	return 0; 
// } 
