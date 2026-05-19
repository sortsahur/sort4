#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NR 4
#define NW 5
#define lock pthread_mutex_lock
#define unlock pthread_mutex_unlock

pthread_mutex_t rcMutex, dataMutex;
int rc = 0, data = 0;

void* reader(void* args){
    int id = *(int*) args;

    while(true){
        lock(&rcMutex);
        rc++;
        if(rc==1) lock(&dataMutex);
        unlock(&rcMutex);
        usleep(800000);

        printf("Reader %d reads %d\n", id, data);
        sleep(1);

        lock(&rcMutex);
        rc--;
        if(rc==0) unlock(&dataMutex);
        unlock(&rcMutex);
        usleep(800000);
    }

    return NULL;
}

void* writer(void* args){
    int id = *(int*) args;
    
    while(true){
        lock(&dataMutex);
        data++;
        printf("Writer %d writes %d\n", id, data);
        unlock(&dataMutex);
        sleep(1);
    }
}

int main(){
    pthread_mutex_init(&rcMutex, NULL);
    pthread_mutex_init(&dataMutex, NULL);

    int read[NR], write[NW];
    pthread_t r[NR], w[NW];

    for(int i = 0; i<NR; i++){
        read[i] = i+1;
        pthread_create(&r[i], NULL, reader, &read[i]);
    }

    for(int i = 0; i<NW; i++){
        write[i] = i+1;
        pthread_create(&w[i], NULL, writer, &write[i]);
    }

    for(int i = 0; i<NR; i++) pthread_join(r[i], NULL);
    for(int i = 0; i<NW; i++) pthread_join(w[i], NULL);

    return 0;
}
