#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

typedef struct 
{
    int ticket;
    int turn;
} lock_t;

void lock_init (lock_t *lock){
    lock->ticket = 0;
    lock->turn = 0;
}

void lock(lock_t *lock){
    int myturn = atomic_fetch_add(&lock->ticket,1);
    while (lock->turn != myturn);
}

void unlock(lock_t *lock){
    lock->turn ++;
}

#define NUM_THREADS 5

lock_t mylock;
int shared_counter = 0;

void *worker (void *arg){
    int id = *(int *)arg;
    // sleep(rand()%2);

    lock(&mylock);
    printf("thread %d entered critical section\n",id);
    int local = shared_counter;
    // sleep(1);
    shared_counter = local + 1e6;
    printf("thread %d leaving critical section.\n",id);
    unlock(&mylock);
    return NULL;
}

int main(){
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    lock_init(&mylock);

    srand(time(NULL));

    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i],NULL, worker, &ids[i]);

    }
    
    for (size_t i = 0; i <NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final shared counter: %d \n",shared_counter);

}