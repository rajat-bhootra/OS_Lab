#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdatomic.h>

typedef struct 
{
    int ticket;
    int turn;
} lock_t;

void lock_init(lock_t *lock){
    lock->ticket = 0;
    lock->turn = 0;
}

void lock(lock_t *lock){
    int myturn = atomic_fetch_add(&lock->ticket, 1);
    while (lock->turn != myturn);
}

void unlock(lock_t *lock){
    lock->turn ++;
}

lock_t mylock;
long long int  counter = 0 ;

void *inc(){
    for (int i = 0; i < 1e6; i++)
    {
        lock(&mylock);
        counter++;
        unlock(&mylock);
    }
    return NULL;
}

int main(){
    int thr;
    printf("Number of threads :");
    scanf("%d",&thr);
    
    pthread_t threads[thr];
    lock_init(&mylock);

    for (int i = 0; i < thr; i++)
    {
        pthread_create(&threads[i], NULL, &inc, NULL);
    }

    for (int i = 0; i < thr; i++)
    {
        pthread_join(threads[i],NULL);
    }
    
    printf("Filnal counter : %lld\n",counter);
    
}