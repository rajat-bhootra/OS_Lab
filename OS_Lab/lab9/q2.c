#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>       // sleep
#include <pthread.h>
#include <semaphore.h>

#define NUM_WORKERS 5

sem_t counter;

void *worker(void *arg) {
    int id = *(int *)arg;
    // Simulate preparatory work
    sleep(1);                         // show non-determinism :contentReference[oaicite:7]{index=7}
    printf("Worker %d: about to signal\n", id);

    // Signal completion
    sem_post(&counter);               // increments semaphore :contentReference[oaicite:8]{index=8}

    // Simulate cleanup after signaling
    sleep(1);                         // this won't affect the join order :contentReference[oaicite:9]{index=9}
    printf("Worker %d: finished work\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_WORKERS];
    int ids[NUM_WORKERS];

    // 1) Initialize semaphore to 0
    if (sem_init(&counter, 0, 0) != 0) {
        perror("sem_init");
        exit(EXIT_FAILURE);
    }

    // 2) Spawn workers (fork)
    for (int i = 0; i < NUM_WORKERS; i++) {
        ids[i] = i + 1;
        if (pthread_create(&threads[i], NULL, worker, &ids[i]) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Simulate parent doing other work before joining
    sleep(1);
    printf("Parent: waiting for %d workers to finish...\n", NUM_WORKERS);

    // 3) Join on workers via semaphore
    for (int i = 0; i < NUM_WORKERS; i++) {
        sem_wait(&counter);            // blocks until a worker does sem_post() :contentReference[oaicite:10]{index=10}
        printf("Parent: received signal %d/%d\n", i + 1, NUM_WORKERS);
    }

    // 4) Cleanup
    for (int i = 0; i < NUM_WORKERS; i++) {
        pthread_join(threads[i], NULL);
    }
    sem_destroy(&counter);

    printf("Parent: all workers have joined. Exiting.\n");
    return 0;
}
    