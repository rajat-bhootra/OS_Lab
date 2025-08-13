#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
    int               count;   // semaphore value
    pthread_mutex_t   lock;    // protects count
    pthread_cond_t    cv;      // for blocking when count == 0
} mysem_t;

// Initialize semaphore to 'value'
int mysem_init(mysem_t *s, int value) {
    s->count = value;                                     
    if (pthread_mutex_init(&s->lock, NULL) != 0)           return -1;  // :contentReference[oaicite:6]{index=6}
    if (pthread_cond_init (&s->cv,   NULL) != 0) {         
        pthread_mutex_destroy(&s->lock);
        return -1;
    }
    return 0;
}

// Wait (P): block if count==0, else decrement
int mysem_wait(mysem_t *s) {
    if (pthread_mutex_lock(&s->lock) != 0)                 return -1;  // :contentReference[oaicite:7]{index=7}
    while (s->count == 0) {                                // handle spurious wakeups :contentReference[oaicite:8]{index=8}
        pthread_cond_wait(&s->cv, &s->lock);               // atomically releases lock while waiting :contentReference[oaicite:9]{index=9}
    }
    s->count--;                                            // consume one unit
    pthread_mutex_unlock(&s->lock);                        // :contentReference[oaicite:10]{index=10}
    return 0;
}

// Post (V): increment count and wake one waiter
int mysem_post(mysem_t *s) {
    if (pthread_mutex_lock(&s->lock) != 0)                 return -1;
    s->count++;                                            // add one unit
    pthread_cond_signal(&s->cv);                           // wake a waiting thread :contentReference[oaicite:11]{index=11}
    pthread_mutex_unlock(&s->lock);
    return 0;
}

// Destroy semaphore resources
int mysem_destroy(mysem_t *s) {
    pthread_mutex_destroy(&s->lock);                       // :contentReference[oaicite:12]{index=12}
    pthread_cond_destroy (&s->cv);
    return 0;
}


mysem_t mutex;
long long int counter = 0;

void *inc(){
    for (int i = 0; i < 1e6; i++)
    {
        mysem_wait(&mutex);
        counter++;
        mysem_post(&mutex);
    }
    return NULL;
}

int main(){
    int thr; 
    printf("threads : ");
    scanf("%d",&thr);

    pthread_t threads[thr];
    mysem_init(&mutex,1);

    for (int i = 0; i < thr; i++)
    {
        pthread_create(&threads[i], NULL, inc, NULL);
    }

    for (int i = 0; i < thr; i++)
    {
        pthread_join(threads[i],NULL);
    }

    printf("final counter : %lld\n",counter);
    
}