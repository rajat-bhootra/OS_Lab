#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>

sem_t sema;

void *runner (void *arg){
     int id = *(int *)arg;

     sleep(1);
     printf("thread %d about to signal\n",id);

     sem_post(&sema);

     sleep(1);
     printf("thread %d done\n",id);

     return NULL;
}

int main(){
    int thr;
    printf("Enter number of threads: ");
    scanf("%d",&thr);

    pthread_t threads[thr];
    int ids[thr];

    sem_init(&sema,0,0);

    for (int i = 0; i < thr; i++)
    {
        ids[i] = i+1;
        pthread_create(&threads[i], NULL, runner, &ids[i]);
    }
  
    sleep(1);
    printf("parent waiting for %d threads to finish\n",thr);

    for (int i = 0; i < thr; i++)
    {
        sem_wait(&sema);
        printf("parent recived signal %d/%d\n",i+1,thr);
    }
    
    for (int i = 0; i < thr; i++)
    {
        pthread_join(threads[i],NULL);
    }

    sem_destroy(&sema);

    printf("all done exiting\n");
}