#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t forks[N];

void* philosopher(void* args) {
    int id = *(int*)args;
    int left = id;
    int right = (id + 1) % N;

    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);

        if (id % 2 == 0) {
            sem_wait(&forks[left]);
            sem_wait(&forks[right]);
        } else {
            sem_wait(&forks[right]);
            sem_wait(&forks[left]);
        }

        printf("Philosopher %d is eating\n", id);
        sleep(1);

        sem_post(&forks[left]);
        sem_post(&forks[right]);
    }
    return NULL;
}

int main() {
    pthread_t t[N];
    int ids[N];

    for (int i = 0; i < N; i++) sem_init(&forks[i], 0, 1);

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&t[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) pthread_join(t[i], NULL);
    for (int i = 0; i < N; i++) sem_destroy(&forks[i]);

    return 0;
}
