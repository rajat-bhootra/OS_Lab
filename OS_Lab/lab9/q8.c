#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef struct {
    sem_t serviceQueue;  // FIFO queue gate
    sem_t resource;      // actual mutex
} fair_mutex_t;

// Initialize the fair mutex
void fair_mutex_init(fair_mutex_t *m) {
    sem_init(&m->serviceQueue, 0, 1);
    sem_init(&m->resource,     0, 1);
}

// Fair, no‑starve lock
void fair_mutex_lock(fair_mutex_t *m) {
    sem_wait(&m->serviceQueue);   // wait your turn
    sem_wait(&m->resource);       // acquire exclusive access
    sem_post(&m->serviceQueue);   // let next in queue proceed
}

// Unlock
void fair_mutex_unlock(fair_mutex_t *m) {
    sem_post(&m->resource);       // release critical section
}

// Destroy the fair mutex
void fair_mutex_destroy(fair_mutex_t *m) {
    sem_destroy(&m->serviceQueue);
    sem_destroy(&m->resource);
}

// Test harness
#define N_THREADS 10
fair_mutex_t mtx;

void* worker(void *arg) {
    int id = *(int*)arg;
    // simulate random arrival
    usleep((rand() % 100) * 1000);
    printf("Thread %d: requesting lock\n", id);

    fair_mutex_lock(&mtx);
      printf("Thread %d: entering critical section\n", id);
      sleep(1);  // simulate work
      printf("Thread %d: leaving critical section\n", id);
    fair_mutex_unlock(&mtx);

    return NULL;
}

int main(void) {
    pthread_t threads[N_THREADS];
    int ids[N_THREADS];
    srand((unsigned)time(NULL));

    fair_mutex_init(&mtx);

    for (int i = 0; i < N_THREADS; i++) {
        ids[i] = i + 1;
        pthread_create(&threads[i], NULL, worker, &ids[i]);
    }
    for (int i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    fair_mutex_destroy(&mtx);
    return 0;
}
