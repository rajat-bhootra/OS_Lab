#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N_READERS 5
#define N_WRITERS 2

sem_t serviceQueue, rmutex, resource;
int   readcount = 0;

void *reader(void *arg) {
    int id = *(int*)arg;
    sleep(rand() % 2);  // simulate pre-entry delay

    printf("Reader %d: waiting\n", id);
    sem_wait(&serviceQueue);        // FIFO arrival queue :contentReference[oaicite:18]{index=18}
    sem_wait(&rmutex);              // protect readcount :contentReference[oaicite:19]{index=19}
    if (++readcount == 1)           // first reader locks resource :contentReference[oaicite:20]{index=20}
        sem_wait(&resource);
    sem_post(&serviceQueue);        // let next thread line up :contentReference[oaicite:21]{index=21}
    sem_post(&rmutex);              // release readcount lock :contentReference[oaicite:22]{index=22}

    // --- CRITICAL SECTION (reading) ---
    printf("Reader %d: reading\n", id);
    sleep(1);
    printf("Reader %d: done\n", id);

    sem_wait(&rmutex);
    if (--readcount == 0)           // last reader releases resource :contentReference[oaicite:23]{index=23}
        sem_post(&resource);
    sem_post(&rmutex);

    return NULL;
}

void *writer(void *arg) {
    int id = *(int*)arg;
    sleep(rand() % 2);  // simulate pre-entry delay

    printf("Writer %d: waiting\n", id);
    sem_wait(&serviceQueue);        // FIFO arrival queue :contentReference[oaicite:24]{index=24}
    sem_wait(&resource);            // exclusive access :contentReference[oaicite:25]{index=25}
    sem_post(&serviceQueue);        // let next thread line up :contentReference[oaicite:26]{index=26}

    // --- CRITICAL SECTION (writing) ---
    printf("Writer %d: writing\n", id);
    sleep(1);
    printf("Writer %d: done\n", id);

    sem_post(&resource);            // release exclusive access :contentReference[oaicite:27]{index=27}
    return NULL;
}

int main(void) {
    pthread_t rt[N_READERS], wt[N_WRITERS];
    int       rid[N_READERS],    wid[N_WRITERS];

    srand((unsigned)time(NULL));
    sem_init(&serviceQueue, 0, 1);
    sem_init(&rmutex,       0, 1);
    sem_init(&resource,     0, 1);

    for (int i = 0; i < N_READERS; i++) {
        rid[i] = i+1;
        pthread_create(&rt[i], NULL, reader, &rid[i]);
    }
    for (int i = 0; i < N_WRITERS; i++) {
        wid[i] = i+1;
        pthread_create(&wt[i], NULL, writer, &wid[i]);
    }
    for (int i = 0; i < N_READERS; i++) pthread_join(rt[i], NULL);
    for (int i = 0; i < N_WRITERS; i++) pthread_join(wt[i], NULL);

    sem_destroy(&serviceQueue);
    sem_destroy(&rmutex);
    sem_destroy(&resource);
    return 0;
}
