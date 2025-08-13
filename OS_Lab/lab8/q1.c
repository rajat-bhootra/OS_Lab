#include <stdio.h>
#include <pthread.h>

int counter=0;
pthread_mutex_t lock;
void* runner(){
    for (int i = 0; i < 1e6; i++){
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(){
    int n;
    printf("number of threads: ");
    scanf("%d", &n);
    pthread_t plist[n];
    pthread_mutex_init(&lock,NULL);
    for (int i = 0; i < n; i++){
        pthread_create(&plist[i],NULL,runner,NULL);
    }
    for (int i = 0; i < n; i++){
        pthread_join(plist[i],NULL);
    }
    printf("counter is %d\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;
}

