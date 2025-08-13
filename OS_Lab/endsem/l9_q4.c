#include <stdio.h>
#include <semaphore.h> 
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

sem_t mutex,gate1,gate2;
int count =0;
int n;
void barrier(void *arg){
     int id = *(int *)arg;
     //p1
     sem_wait(&mutex);
     count ++;
     if (count == n){
        sem_wait(&gate2);
        sem_post(&gate1);
     }
     sem_post(&mutex);
     
     sleep(rand()%2);
     sem_wait(&gate1);
     sem_post(&gate1);

     printf("thread %d pass the phase1\n",id);

     sem_wait(&mutex);
     count --;
     if (count == 0){
        sem_wait(&gate1);
        sem_post(&gate2);
     }
     sem_post(&mutex);
     
     sleep(rand()%2);
     sem_wait(&gate2);
     sem_post(&gate2);

     printf("thread %d pass the phase2\n",id);
}

void *thread_func(void *arg){
    int id = *(int *)arg;
    printf("thread %d before phase1\n",id);
    sleep(rand()%2);
    barrier(&id);
    printf("thread %d passed the barrier\n",id);
    return NULL;
}

int main(int argc, char const *argv[])
{
    n = atoi(argv[1]);

    pthread_t threads[n];
    int ids[n];
    sem_init(&mutex,0,1);
    sem_init(&gate1,0,0);
    sem_init(&gate2,0,1);
    
    for (int i = 0; i < n; i++)
    {
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }
    
    for (int i = 0; i < n; i++)
    {
        pthread_join(threads[i],NULL);
    }
    
    return 0;
}

