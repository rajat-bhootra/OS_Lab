#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>      // sleep()
#include <pthread.h>
#include <semaphore.h>

#define N_THREADS 5

sem_t mutex, turnstile, turnstile2;
int count = 0;

void barrier() {
    // Phase 1: arrive
    sem_wait(&mutex);
    count++;
    if (count == N_THREADS) {
        // Last thread locks turnstile2 and opens turnstile
        sem_wait(&turnstile2);
        sem_post(&turnstile);
    }
    sem_post(&mutex);

    // All threads wait here until turnstile opened
    sleep(1);               // simulate arbitrary delay before P1 release
    sem_wait(&turnstile);
    sem_post(&turnstile);

    // --- Critical point between P1 and P2 ---
    printf("Thread %lu passed P1\n", pthread_self());

    // Phase 2: depart
    sem_wait(&mutex);
    count--;
    if (count == 0) {
        // Last thread locks turnstile and opens turnstile2
        sem_wait(&turnstile);
        sem_post(&turnstile2);
    }
    sem_post(&mutex);

    // All threads wait here until turnstile2 opened
    sleep(1);               // simulate arbitrary delay before P2 release
    sem_wait(&turnstile2);
    sem_post(&turnstile2);

    // Now every thread has seen that all others reached the barrier
    printf("Thread %lu passed P2\n", pthread_self());
}

void* thread_func(void* arg) {
    size_t id = (size_t)arg;
    printf("Thread %zu before P1\n", id);
    sleep(1);               // simulate work before the barrier
    barrier();
    printf("Thread %zu after barrier\n", id);
    return NULL;
}

int main(void) {
    pthread_t threads[N_THREADS];

    // Initialize semaphores
    sem_init(&mutex,     0, 1);   // protects count :contentReference[oaicite:4]{index=4}
    sem_init(&turnstile, 0, 0);   // initially closed :contentReference[oaicite:5]{index=5}
    sem_init(&turnstile2,0, 1);   // initially open  :contentReference[oaicite:6]{index=6}

    // Spawn threads
    for (size_t i = 0; i < N_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_func, (void*)i);
    }

    // Join threads
    for (size_t i = 0; i < N_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Clean up
    sem_destroy(&mutex);
    sem_destroy(&turnstile);
    sem_destroy(&turnstile2);

    return 0;
}
