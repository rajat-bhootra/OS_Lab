#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semB;

void *threadA(void* arg){
    printf("thread A : before rendezveous\n");
    sleep(1);

    sem_post(&semA);
    printf("thread A : signaled on semA waiting on semB\n");

    sem_wait(&semB);
    printf("thread A : past rendezveous\n");
    
    sleep(1);
    printf("thread A : done\n");

    return NULL;
}

void *threadB(void* arg){
    printf("thread B : before rendezveous\n");
    sleep(1);

    sem_post(&semB);
    printf("thread B : signaled on semB waiting on semA\n");

    sem_wait(&semA);
    printf("thread B : past rendezveous\n");
    
    sleep(1);
    printf("thread B : done\n");

    return NULL;
}

int main(){
    pthread_t tA,tB;
    
    sem_init(&semA,0,0);
    sem_init(&semB,0,0);

    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);

    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    sem_destroy(&semA);
    sem_destroy(&semB);
}