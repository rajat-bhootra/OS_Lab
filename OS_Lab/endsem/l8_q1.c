#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
long long int counter = 0;

void *inc (){
    for (int i = 0; i < 1e6; i++)
    {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    int thr; 
    printf("threads : ");
    scanf("%d",&thr);

    pthread_t threads[thr];

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