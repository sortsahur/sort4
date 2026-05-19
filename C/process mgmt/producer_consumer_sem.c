#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10
#define NP 3
#define NC 3

sem_t empty, full;
pthread_mutex_t mutex;
int buffer[N];
int ip = 0, op = 0;

void* producer(void* args) {
    int id = *(int*)args;
    while (1) {
        int val = rand() % 100;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[ip] = val;
        printf("Producer %d produces %d at position %d\n", id, val, ip);
        ip = (ip + 1) % N;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* args) {
    int id = *(int*)args;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        printf("Consumer %d consumes %d from position %d\n", id, buffer[op], op);
        op = (op + 1) % N;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
    return NULL;
}

int main() {
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t pt[NP], ct[NC];
    int pid[NP], cid[NC];

    for (int i = 0; i < NP; i++) {
        pid[i] = i + 1;
        pthread_create(&pt[i], NULL, producer, &pid[i]);
    }
    for (int i = 0; i < NC; i++) {
        cid[i] = i + 1;
        pthread_create(&ct[i], NULL, consumer, &cid[i]);
    }

    for (int i = 0; i < NP; i++) pthread_join(pt[i], NULL);
    for (int i = 0; i < NC; i++) pthread_join(ct[i], NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}
