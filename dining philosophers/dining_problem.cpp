#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

/*
    CECS 326 Project 2: The Dining Philosophers Problem
    @authors: Nolan Nguyen, Jordan Havelaar
    @description: This program creates a 
    @version 1.0.0
    @date October 12, 2024

*/

#define NUM_PHILOSOPHERS 5

pthread_mutex_t mutex;
pthread_cond_t cond_vars[NUM_PHILOSOPHERS];
int state[NUM_PHILOSOPHERS]; // 0: thinking, 1: hungry, 2: eating

// Function prototypes
void create_problem();
void test(int philosopher_number);
void pickup_forks(int philosopher_number);
void return_forks(int philosopher_number);
void* philosopher(void* num);
long get_time_in_ms();

int main() {
    srand(time(NULL));
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int philosopher_numbers[NUM_PHILOSOPHERS];

    create_problem();

    // Print initial state of all philosophers
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        printf("Philosopher %d is thinking.\n", i);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_numbers[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &philosopher_numbers[i]);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}

void create_problem() {
    pthread_mutex_init(&mutex, NULL); //mutex_init
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_cond_init(&cond_vars[i], NULL); //cond_init
        state[i] = 0; // Initially, all philosophers are thinking
    }
}

void test(int philosopher_number) {
    if (state[philosopher_number] == 1 && 
        state[(philosopher_number + 4) % NUM_PHILOSOPHERS] != 2 && //check if both chopsticks are not picked up by a philsopher
        state[(philosopher_number + 1) % NUM_PHILOSOPHERS] != 2) {
        state[philosopher_number] = 2;
        pthread_cond_signal(&cond_vars[philosopher_number]);
    }
}

void pickup_forks(int philosopher_number) {
    pthread_mutex_lock(&mutex);
    if (state[philosopher_number] != 1) {
        state[philosopher_number] = 1; // Hungry
        printf("Philosopher %d is hungry.\n", philosopher_number);
    }
    test(philosopher_number);
    while (state[philosopher_number] != 2) {
        pthread_cond_wait(&cond_vars[philosopher_number], &mutex);
    }
    printf("Philosopher %d is eating.\n", philosopher_number);
    pthread_mutex_unlock(&mutex);
}

void return_forks(int philosopher_number) {
    pthread_mutex_lock(&mutex);
    if (state[philosopher_number] != 0) {
        state[philosopher_number] = 0; // Thinking
        printf("Philosopher %d is thinking.\n", philosopher_number);
    }
    test((philosopher_number + 4) % NUM_PHILOSOPHERS);
    test((philosopher_number + 1) % NUM_PHILOSOPHERS);
    pthread_mutex_unlock(&mutex);
}

void* philosopher(void* num) {
    int philosopher_number = *(int*)num;
    long start_time, end_time;

    while (1) {
        // Thinking
        start_time = get_time_in_ms();
        if (state[philosopher_number] != 0) {
            state[philosopher_number] = 0;
            printf("Philosopher %d is thinking.\n", philosopher_number);
        }
        sleep(rand() % 3 + 1);
        end_time = get_time_in_ms();
        printf("Philosopher %d spent %ld ms thinking.\n", philosopher_number, end_time - start_time);

        // Hungry
        pickup_forks(philosopher_number);

        // Eating
        start_time = get_time_in_ms();
        sleep(rand() % 3 + 1);
        end_time = get_time_in_ms();
        printf("Philosopher %d spent %ld ms eating.\n", philosopher_number, end_time - start_time);

        // Done eating
        return_forks(philosopher_number);
    }
    return NULL;
}

long get_time_in_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); //clock handling in ctime
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}
