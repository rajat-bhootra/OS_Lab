// rw_lock.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_READ_ITER 10

sem_t mutex;        // protects reader_count
sem_t write_sem;    // writers’ lock
int reader_count = 0;

void reader(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < NUM_READ_ITER; i++) {
        // Entry section for reader
        sem_wait(&mutex);
        reader_count++;
        if (reader_count == 1) {
            // first reader locks out writers
            sem_wait(&write_sem);
        }
        sem_post(&mutex);

        // Critical section
        printf("Reader %d: entered (readers=%d)\n", id, reader_count);
        sleep(1);  // simulate read work
        printf("Reader %d: leaving\n", id);

        // Exit section for reader
        sem_wait(&mutex);
        reader_count--;
        if (reader_count == 0) {
            // last reader releases writers
            sem_post(&write_sem);
        }
        sem_post(&mutex);

        // Think time before next read
        sleep(1);
    }
    free(arg);
    return;
}

void writer(void* arg) {
    (void)arg;
    // try to write once—but will loop until it succeeds
    printf("Writer: attempting to acquire lock...\n");
    sem_wait(&write_sem);
    // Critical section
    printf("Writer: entered\n");
    sleep(1);  // simulate write work
    printf("Writer: leaving\n");
    sem_post(&write_sem);

    return;
}

int main() {
    pthread_t readers[NUM_READERS], w;

    sem_init(&mutex, 0, 1);
    sem_init(&write_sem, 0, 1);

    // start readers
    for (int i = 0; i < NUM_READERS; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&readers[i], NULL, (void*)reader, id);
        usleep(100000);  // stagger their start slightly
    }

    // give readers a head start
    sleep(1);
    // start writer
    pthread_create(&w, NULL, (void*)writer, NULL);

    // wait for all readers
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }
    // at this point readers have finished; writer can proceed
    pthread_join(w, NULL);

    sem_destroy(&mutex);
    sem_destroy(&write_sem);
    return 0;
}
