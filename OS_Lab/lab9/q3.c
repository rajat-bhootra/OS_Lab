#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      // sleep()
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semB;

// Thread A’s rendezvous function
void* threadA(void* arg) {
    printf("Thread A: before rendezvous\n");
    sleep(1);                    // simulate work before rendezvous :contentReference[oaicite:5]{index=5}
    sem_post(&semA);             // signal arrival :contentReference[oaicite:6]{index=6}
    printf("Thread A: signaled semA, now waiting on semB\n");
    sem_wait(&semB);             // wait for Thread B :contentReference[oaicite:7]{index=7}
    printf("Thread A: past rendezvous\n");
    sleep(1);                    // simulate work after rendezvous :contentReference[oaicite:8]{index=8}
    printf("Thread A: done\n");
    return NULL;
}

// Thread B’s rendezvous function
void* threadB(void* arg) {
    printf("Thread B: before rendezvous\n");
    sleep(1);                    // simulate work before rendezvous :contentReference[oaicite:9]{index=9}
    sem_post(&semB);             // signal arrival :contentReference[oaicite:10]{index=10}
    printf("Thread B: signaled semB, now waiting on semA\n");
    sem_wait(&semA);             // wait for Thread A :contentReference[oaicite:11]{index=11}
    printf("Thread B: past rendezvous\n");
    sleep(1);                    // simulate work after rendezvous :contentReference[oaicite:12]{index=12}
    printf("Thread B: done\n");
    return NULL;
}

int main(void) {
    pthread_t tA, tB;

    // Initialize both semaphores to 0 (shared between threads) :contentReference[oaicite:13]{index=13}
    if (sem_init(&semA, 0, 0) < 0 || sem_init(&semB, 0, 0) < 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // Create threads A and B :contentReference[oaicite:14]{index=14}
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);

    // Wait for both to finish (cleanup) :contentReference[oaicite:15]{index=15}
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);

    // Destroy semaphores :contentReference[oaicite:16]{index=16}
    sem_destroy(&semA);
    sem_destroy(&semB);

    printf("Main: all threads done\n");
    return 0;
}
