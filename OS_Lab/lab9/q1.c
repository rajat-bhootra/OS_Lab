/* Simple Producer-Consumer using Condition Variables */
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>

#define ITEMS 1000
#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE], count = 0, in = 0, out = 0;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    for (int i = 1; i <= ITEMS; i++) {
        pthread_mutex_lock(&mtx);
        while (count == BUFFER_SIZE)
            pthread_cond_wait(&not_full, &mtx);
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 0; i < ITEMS; i++) {
        pthread_mutex_lock(&mtx);
        while (count == 0)
            pthread_cond_wait(&not_empty, &mtx);
        int item = buffer[out];
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;
        count--;
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mtx);
    }
    return NULL;
}

int main() {
    int n;
    printf("number of threads : ");
    scanf("%d",&n);
    pthread_t prod[n], cons[n];
    for (int i = 0; i < n; i++)
    {
        pthread_create(&prod[i], NULL, producer, NULL);
        pthread_create(&cons[i], NULL, consumer, NULL);
    }
    
    for (int i = 0; i < n; i++)
    {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }
    return 0;
}
