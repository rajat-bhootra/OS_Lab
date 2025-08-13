#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int* producer_done;
int* buffer;
int buffer_size, in_size = 0, out_size = 0;

sem_t empty;
sem_t full;
sem_t mutex;

void* producer(void* ar){
    int id = *(int*)ar;
    free(ar);
    
    for(int i = 0; i < 1000; i++){
        int item = (id + 1) * i;

        sem_wait(&empty);       // Wait for empty slot
        sem_wait(&mutex);       // Enter critical section

        buffer[in_size] = item;
        in_size = (in_size + 1) % buffer_size;

        sem_post(&mutex);       // Exit critical section
        sem_post(&full);        // Signal full slot available
    }

    // Signal producer is done
    sem_wait(&mutex);
    producer_done[id] = 1;
    sem_post(&mutex);

    return NULL;
}

void* consumer(void* ar){
    int id = *(int*)ar;
    int base = id + 1;
    free(ar);

    while(1){
        sem_wait(&full);        // Wait until something to consume
        sem_wait(&mutex);       // Enter critical section

        // Check if all producers are done and nothing left to consume
        int all_done = 1;
        for(int i = 0; i < buffer_size; i++){
            if(producer_done[i] == 0){
                all_done = 0;
                break;
            }
        }

        // If all producers are done and buffer is empty, exit
        if(all_done && in_size == out_size){
            sem_post(&mutex);
            sem_post(&full); // Allow other consumers to check
            break;
        }

        int item = buffer[out_size];

        // Only consume if divisible by base
        if(item % base == 0){
            printf("Consumer %d consumed: %d\n", id, item);
            out_size = (out_size + 1) % buffer_size;
            sem_post(&empty);   // Signal one slot empty now
        } else {
            // Skip item: don't advance out_size or post empty
            // We'll try again next loop
            sem_post(&full); // Let another consumer try
        }

        sem_post(&mutex);
        usleep(100);
    }

    return NULL;
}

int main(){
    int n;
    scanf("%d", &n);

    pthread_t prod[n], cons[n];
    buffer_size = n;
    buffer = malloc(sizeof(int) * n);
    producer_done = malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++) producer_done[i] = 0;

    // Initialize semaphores
    sem_init(&empty, 0, n);    // All buffer slots are empty
    sem_init(&full, 0, 0);     // No full slots initially
    sem_init(&mutex, 0, 1);    // Mutex as binary semaphore

    // Create producers
    for(int i = 0; i < n; i++){
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&prod[i], NULL, producer, id);
    }

    // Create consumers
    for(int i = 0; i < n; i++){
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&cons[i], NULL, consumer, id);
    }

    for(int i = 0; i < n; i++){
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    // for(int i = 0; i < n; i++){
    // }

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);
    free(buffer);
    free(producer_done);

    printf("Finished\n");
    return 0;
}