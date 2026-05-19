#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define NR 4
#define NW 5
typedef sem_t sem;
sem rcMutex, dataMutex;
int rc = 0, data = 0;

void down(sem *s){sem_wait(s);}
void up(sem *s){sem_post(s);}

void* reader(void* args){
    int id = *(int*) args;

    while(true){
        down(&rcMutex);
        rc++;
        if(rc == 1) down(&dataMutex);
        up(&rcMutex);

        up(&dataMutex);
        printf("Reader %d reads: %d\n", id, data);
        sleep(1);
        down(&dataMutex);

        down(&rcMutex);
        rc--;
        if(rc == 0) up(&dataMutex);
        up(&rcMutex);
    }

    return NULL;
}

void* writer(void* args){
    int id = *(int*) args;

    while(true){
        down(&dataMutex);
        data++;
        printf("Writer %d writes: %d\n", id, data);
        sleep(1);
        up(&dataMutex);
    }

    return NULL;
}

int main(){
    sem_init(&rcMutex, 0, 1);
    sem_init(&dataMutex, 0, 1);

    int read[NR], write[NW];
    pthread_t r[NR], w[NW];

    for(int i = 0; i<NW; i++){
        read[i] = i+1;
        pthread_create(&r[i], NULL, reader, &read[i]);
    }

    for(int i = 0; i<NR; i++){
        write[i] = i+1;
        pthread_create(&w[i], NULL, writer, &write[i]);
    }

    for(int i = 0; i<NW; i++) pthread_join(w[i], NULL);
    for(int i = 0; i<NR; i++) pthread_join(r[i], NULL);

    return 0;
}