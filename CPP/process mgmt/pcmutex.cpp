#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <bits/stdc++.h>

#define N 50
#define NP 7
#define NC 6
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock

pthread_mutex_t mutex;
int buffer[N];
int ip = 0, op = 0, count = 0;

void* producer(void* args){
    int id = *(int*) args;
    
    while(true){
        int val = rand()%50+id;

        lock(&mutex);
        if(count == N){
            unlock(&mutex);
            continue;
        }

        buffer[ip] = val;
        printf("Producer %d produces %d at location %d\n", id, val, ip);
        sleep(1);
        ip = (ip+1)%N;
        count++;

        unlock(&mutex);
        usleep(300000);
    }

    return NULL;
}

void* consumer(void* args){
    int id = *(int*) args;
    
    while(true){
        lock(&mutex);   
        if(count == 0){
            unlock(&mutex);
            continue;
        }

        printf("Consumer %d consumes %d from location %d\n", id, buffer[op], op);
        sleep(1);
        op = (op+1)%N;
        count--;
        unlock(&mutex);
        sleep(1);
    }

    return NULL;
}

int main(){
    pthread_mutex_init(&mutex, NULL);

    int prod[NP], cons[NC];
    pthread_t p[NP], c[NC];

    for(int i = 0; i<NP; i++){
        prod[i] = i+1;
        pthread_create(&p[i], NULL, producer, &prod[i]);
    }

    for(int i = 0; i<NC; i++){
        cons[i] = i+1;
        pthread_create(&c[i], NULL, consumer, &cons[i]);
    }

    for(int i = 0; i<NP; i++) pthread_join(p[i], NULL);
    for(int i = 0; i<NC; i++) pthread_join(c[i], NULL);

    return 0;
}