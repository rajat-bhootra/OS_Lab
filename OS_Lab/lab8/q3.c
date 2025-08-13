// concurrent_list.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

#define NUM_THREADS 5   
#define NUM_OPS     100000

typedef struct node {
    int key;
    struct node* next;
} node_t;

typedef struct {
    node_t* head;
    pthread_mutex_t lock; // Used only in concurrent version
    int use_lock;         // Flag to use locking or not
} list_t;

void list_init(list_t* list, int use_lock) {
    list->head = NULL;
    list->use_lock = use_lock;
    if (use_lock) pthread_mutex_init(&list->lock, NULL);
}

int list_insert(list_t* list, int key) {
    if (list->use_lock) pthread_mutex_lock(&list->lock);
    node_t* new = malloc(sizeof(node_t));
    if (!new) return -1;
    new->key = key;
    new->next = list->head;
    list->head = new;
    if (list->use_lock) pthread_mutex_unlock(&list->lock);
    return 0;
}

int list_lookup(list_t* list, int key) {
    if (list->use_lock) pthread_mutex_lock(&list->lock);
    node_t* curr = list->head;
    while (curr) {
        if (curr->key == key) {
            if (list->use_lock) pthread_mutex_unlock(&list->lock);
            return 1;
        }
        curr = curr->next;
    }
    if (list->use_lock) pthread_mutex_unlock(&list->lock);
    return 0;
}

void* thread_fn(void* arg) {
    list_t* list = (list_t*)arg;
    for (int i = 0; i < NUM_OPS; i++) {
        list_insert(list, i);
        list_lookup(list, i);
    }
    return NULL;
}

double current_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1e6;
}

void run_benchmark(int use_lock) {
    list_t list;
    list_init(&list, use_lock);
    pthread_t threads[NUM_THREADS];

    double start = current_time();
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, thread_fn, &list);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    double end = current_time();
    printf("%s version: %.2f seconds\n",
           use_lock ? "Concurrent" : "Regular", end - start);
}

int main() {
    printf("Benchmarking with %d threads, %d ops each:\n\n", NUM_THREADS, NUM_OPS);

    run_benchmark(0); // Regular (no locking)
    run_benchmark(1); // Concurrent (with locking)

    return 0;
}
