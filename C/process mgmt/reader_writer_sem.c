#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NR 3
#define NW 2

sem_t rsem, wsem;
pthread_mutex_t mutex;
int readcount = 0;
int shared_data = 0;

void* reader(void* args) {
    int id = *(int*)args;
    while (1) {
        sem_wait(&rsem);
        pthread_mutex_lock(&mutex);
        readcount++;
        if (readcount == 1) sem_wait(&wsem);
        pthread_mutex_unlock(&mutex);
        sem_post(&rsem);

        printf("Reader %d reads data: %d\n", id, shared_data);
        sleep(1);

        pthread_mutex_lock(&mutex);
        readcount--;
        if (readcount == 0) sem_post(&wsem);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

void* writer(void* args) {
    int id = *(int*)args;
    while (1) {
        sem_wait(&wsem);
        shared_data++;
        printf("Writer %d writes data: %d\n", id, shared_data);
        sleep(1);
        sem_post(&wsem);
        sleep(1);
    }
    return NULL;
}

int main() {
    sem_init(&rsem, 0, 1);
    sem_init(&wsem, 0, 1);
    pthread_mutex_init(&mutex, NULL);

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

    sem_destroy(&rsem);
    sem_destroy(&wsem);
    pthread_mutex_destroy(&mutex);
    return 0;
}
