#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define N 10
#define NP 3
#define NC 3

pthread_mutex_t mutex;
int buffer[N];
int ip = 0, op = 0, count = 0;

void* producer(void* args) {
    int id = *(int*)args;
    while (1) {
        int val = rand() % 100;
        pthread_mutex_lock(&mutex);
        if (count == N) {
            pthread_mutex_unlock(&mutex);
            usleep(100000);
            continue;
        }
        buffer[ip] = val;
        printf("Producer %d produces %d at position %d\n", id, val, ip);
        ip = (ip + 1) % N;
        count++;
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* consumer(void* args) {
    int id = *(int*)args;
    while (1) {
        pthread_mutex_lock(&mutex);
        if (count == 0) {
            pthread_mutex_unlock(&mutex);
            usleep(100000);
            continue;
        }
        printf("Consumer %d consumes %d from position %d\n", id, buffer[op], op);
        op = (op + 1) % N;
        count--;
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int main() {
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

    pthread_mutex_destroy(&mutex);
    return 0;
}
