#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

typedef struct{
    int ticket;
    int turn;
}lock_t;

void lock_init(lock_t *l){
    l->ticket = 0;
    l->turn = 0;
}

void lock(lock_t *l){
    int myturn = atomic_fetch_add(&l->ticket,1);
    while(l->turn != myturn);
}

void unlock(lock_t *l){
    l->turn ++;
}

lock_t mylock;
long long int counter = 0;

void *l_inc (){
    long long int l_counter = 0;
    for (int i = 0; i < 1e6; i++)
    {
        l_counter++;
        if (l_counter > 1e3){
            lock(&mylock);
            counter += l_counter;
            unlock(&mylock);
            l_counter = 0;
        }
    }
    lock(&mylock);
    counter+=l_counter; 
    unlock(&mylock);
    return NULL;
}

int main(){
    int thr; 
    printf("threads : ");
    scanf("%d",&thr);

    pthread_t threads[thr];
    lock_init(&mylock);

    for (int i = 0; i < thr; i++)
    {
        pthread_create(&threads[i], NULL, l_inc, NULL);
    }

    for (int i = 0; i < thr; i++)
    {
        pthread_join(threads[i],NULL);
    }

    printf("final counter : %lld\n",counter);
}
