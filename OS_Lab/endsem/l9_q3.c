#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

sem_t semA, semB;

void *threadA(){
    printf("Thread A : before renda\n");
    sleep(rand()%2);

    sem_post(&semA);
    printf("Thread A : signaled on semA now waiting on semB\n");

    sem_wait(&semB);
    printf("Threads A : after renda\n");

    sleep(rand()%2);
    printf("Thread A : Done\n");
    return NULL;
}

void *threadB(){
    printf("Thread B : before renda\n");
    sleep(rand()%2);

    sem_post(&semB);
    printf("Thread B : signaled on semB now waiting on semA\n");

    sem_wait(&semA);
    printf("Threads B : after renda\n");

    sleep(rand()%2);
    printf("Thread B : Done\n");
    return NULL;
}

int main(){
    pthread_t thrA, thrB;
    
    sem_init(&semA,0,0);
    sem_init(&semB,0,0);

    pthread_create(&thrA, NULL, threadA, NULL);
    pthread_create(&thrB, NULL, threadB, NULL);

    pthread_join(thrA, NULL);
    pthread_join(thrB, NULL);

    printf ("Done !!!\n");

    return 0;
}