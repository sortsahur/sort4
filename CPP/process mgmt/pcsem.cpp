#include <stdio.h>
#include <bits/stdc++.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 50
#define NP 6
#define NC 7
typedef sem_t sem;
sem mutex, nempty, nfull = 0;

int buffer[N];
int ip = 0, op = 0;

void down(sem* s){sem_wait(s);}
void up(sem* s){sem_post(s);}

void* producer(void* args){
    int id = *(int*) args;

    while(true){
        int item = rand()%50+id;

        down(&nempty);
        down(&mutex);
        
        buffer[ip] = item;
        printf("Producer %d produces item %d at location %d\n", id, item, ip);
        sleep(1);
        ip = (ip+1)%N;

        up(&mutex);
        up(&nfull);
    }
}

void* consumer(void* args){
    int id = *(int*) args;

    while(true){
        down(&nfull);
        down(&mutex);

        printf("Consumer %d consumes item %d from location %d\n", id, buffer[op], op);
        sleep(1);
        buffer[op] = -1;
        op = (op+1)%N;

        up(&mutex);
        up(&nempty);
    }
}

int main(){
    sem_init(&mutex, 0, 1);
    sem_init(&nempty, 0, N);
    sem_init(&nfull, 0, 0);

    int prod[NP], cons[NC];
    pthread_t p[NP], c[NC];

    for(int i = 0; i < NP; i++){
        prod[i] = i+1;
        pthread_create(&p[i], NULL, producer, &prod[i]);
    }

    for(int i = 0; i < NC; i++){
        cons[i] = i+1;
        pthread_create(&c[i], NULL, consumer, &cons[i]);
    }

    for(int i = 0; i < NP; i++){
        pthread_join(p[i], NULL);
    }

    for(int i = 0; i < NC; i++){
        cons[i] = i+1;
        pthread_join(c[i], NULL);
    }

    return 0;
}