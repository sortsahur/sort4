#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define THINKING 0
#define HUNGRY 1
#define EATING 2

typedef sem_t sem;
sem mutex, s[N];
int state[N];

void down(sem* s){sem_wait(s);}
void up(sem* s){sem_post(s);}

void think(int id){
    // state[id] = THINKING; - dont do this here we're changing state in put forks ONLY when we're locking mutex
    printf("Philosopher %d is THINKING\n", id+1);
    sleep(1);
}

void eat(int id){
    printf("Philosopher %d is EATING\n", id+1);
    sleep(1);
}

void test(int id){
    if(state[id] == HUNGRY && state[(id+N-1)%N] != EATING && state[(id+1)%N] != EATING){
        state[id] = EATING;
        up(&s[id]);
    }
}

void take_forks(int id){
    down(&mutex);
    state[id] = HUNGRY;
    printf("Philosopher %d is HUNGRY\n", id+1);
    test(id);
    up(&mutex);

    sleep(1);

    down(&s[id]);
}

void put_forks(int id){
    down(&mutex);
    state[id] = THINKING;
    printf("Philosopher %d is THINKING\n", id+1);
    test((id-1)%N);
    test((id+1)%N);
    up(&mutex);

    sleep(1);
}

void* philosopher(void* args){
    int id = *(int*) args;

    while(true){
        think(id);
        take_forks(id);
        eat(id);
        put_forks(id);
    }

    return NULL;
}

int main(){
    sem_init(&mutex, 0, 1);
    for(int i = 0; i<N; i++) sem_init(&s[i], 0, 0); //initially 0 bc they cant eat at first

    int phils[N];
    pthread_t p[N];

    for(int i = 0; i<N; i++){
        phils[i] = i;
        pthread_create(&p[i], NULL, philosopher, &phils[i]);
    }

    for(int i = 0; i<N; i++) pthread_join(p[i], NULL);

    return 0;
}