#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NR 3
#define NW 2

pthread_mutex_t rmutex, wmutex;
int readcount = 0;
int shared_data = 0;

void* reader(void* args) {
    int id = *(int*)args;
    while (1) {
        pthread_mutex_lock(&rmutex);
        readcount++;
        if (readcount == 1) pthread_mutex_lock(&wmutex);
        pthread_mutex_unlock(&rmutex);

        printf("Reader %d reads data: %d\n", id, shared_data);
        sleep(1);

        pthread_mutex_lock(&rmutex);
        readcount--;
        if (readcount == 0) pthread_mutex_unlock(&wmutex);
        pthread_mutex_unlock(&rmutex);
        sleep(1);
    }
    return NULL;
}

void* writer(void* args) {
    int id = *(int*)args;
    while (1) {
        pthread_mutex_lock(&wmutex);
        shared_data++;
        printf("Writer %d writes data: %d\n", id, shared_data);
        sleep(1);
        pthread_mutex_unlock(&wmutex);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_mutex_init(&rmutex, NULL);
    pthread_mutex_init(&wmutex, NULL);

    pthread_t rt[NR], wt[NW];
    int rid[NR], wid[NW];

    for (int i = 0; i < NR; i++) {
        rid[i] = i + 1;
        pthread_create(&rt[i], NULL, reader, &rid[i]);
    }
    for (int i = 0; i < NW; i++) {
        wid[i] = i + 1;
        pthread_create(&wt[i], NULL, writer, &wid[i]);
    }

    for (int i = 0; i < NR; i++) pthread_join(rt[i], NULL);
    for (int i = 0; i < NW; i++) pthread_join(wt[i], NULL);

    pthread_mutex_destroy(&rmutex);
    pthread_mutex_destroy(&wmutex);
    return 0;
}
