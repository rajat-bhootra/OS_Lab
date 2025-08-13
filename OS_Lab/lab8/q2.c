#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>

long long int counter=0;
pthread_mutex_t lock;
void* runner(){
    long long int lcounter=0;
    for (int i = 0; i < 1e6; i++){
         lcounter++;
         if (lcounter > 1e3){
            pthread_mutex_lock(&lock);
            counter+=lcounter;
            pthread_mutex_unlock(&lock);
            lcounter=0;
         }
    }
    pthread_mutex_lock(&lock);
    counter+=lcounter;
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(){
    int n;
    printf("Enter number of threads: ");
    scanf("%d", &n);
    pthread_t plist[n];
    pthread_mutex_init(&lock,NULL);
    for (int i = 0; i < n; i++){
        pthread_create(&plist[i],NULL,runner,NULL);
    }
    for (int i = 0; i < n; i++){
        pthread_join(plist[i],NULL);
    }
    printf("counter is %lld\n", counter);
    pthread_mutex_destroy(&lock);
    return 0;
}