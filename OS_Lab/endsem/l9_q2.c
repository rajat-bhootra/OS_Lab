#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

sem_t sem;

void *runner(void *arg){
    int id = *(int *)arg;

    sleep(rand()%2);
    printf("Thread %d : about to signal\n",id);

    sem_post(&sem);

    sleep(rand()%2);
    printf("Thread %d : Done\n",id);

    return NULL;
}

int main(){
    int n;
    printf("Num threads : ");
    scanf("%d",&n);

    pthread_t threads[n];
    int id[n];
    sem_init(&sem ,0 ,0);


    for (int  i = 0; i < n; i++)
    {
        id[i] = i+1;
        pthread_create(&threads[i], NULL, runner, &id[i]);
    }

    printf("parent waiting for %d threads to signal\n",n);

    for (int i = 0; i < n; i++)
    {
        printf("recived signal %d/%d \n",i+1, n);
        sem_wait(&sem);
    }

    for (int  i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    printf("DONE !!!\n");
    return 1;
    
}