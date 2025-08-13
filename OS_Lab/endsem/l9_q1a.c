#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cons = PTHREAD_COND_INITIALIZER; 
pthread_cond_t prod = PTHREAD_COND_INITIALIZER;


int *prod_done;
int *buffer;
int N;
int B;
int in = 0 , out = 0 , count = 0; 

void *producer(void *arg){
    int id = *(int *)arg;
    for (int i = 1; i <= 1000; i++)
    {
        pthread_mutex_lock(&lock);
        while (count == B)
        {
            pthread_cond_wait(&cons,&lock);
        }
        buffer[in] = (id+1)*i;
        in = (in+1)%B;
        count++;
        pthread_cond_broadcast(&prod);
        pthread_mutex_unlock(&lock);
    }
    pthread_mutex_lock(&lock);
    prod_done[id] = 1;
    pthread_cond_broadcast(&prod);
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

void *consumer(void *arg){
    int id = *(int *)arg;
    while (1)
    {
        pthread_mutex_lock(&lock);
        while (count == 0)
        {
            int all_done = 1;
            for (int i = 1; i < N; i++)
            {
                if (!prod_done[i])
                {
                    all_done = 0;
                    break;
                }
            }
            if (all_done){
                pthread_mutex_unlock(&lock);
                break;
            }
            pthread_cond_wait(&prod,&lock);
        }

        if (count == 0){
            pthread_mutex_unlock(&lock);
            break;
        }

        int value = buffer[out];
        if (value % (id +1) == 0){
            printf("Consumer %d : consumed %d\n",id,value);
            out = (out+1)%B;
            count --;
            pthread_cond_signal(&cons);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc , char *argv[]){
    if (argc != 3){
        printf("Usage : %s <N> <B>\n",argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    B = atoi(argv[2]);

    prod_done = calloc(N,sizeof(int));
    buffer = (int *)malloc(B*sizeof(int));

    pthread_t consu[N], produ[N];

    for (int i = 1; i <= N; i++)
    {
        int *id = malloc(sizeof(int));
        *id =i;
        pthread_create(&produ[i], NULL, producer, id);
    }

    for (int i = 1; i <= N; i++)
    {
        int *id = malloc(sizeof(int));
        *id =i;
        pthread_create(&consu[i], NULL, consumer, id);
    }

    for (int i = 1; i <= N; i++)
    {
        pthread_join(produ[i],NULL);
        pthread_join(consu[i],NULL);
    }

    printf("DONE!!!\n");
    return 0;
    
}