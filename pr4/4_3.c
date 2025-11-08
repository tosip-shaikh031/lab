/*4.3 The Producer generates an integer between 0 and 9 (inclusive), stores it in a file.  To make the 
synchronization problem more interesting, the Producer sleeps for a random amount of time between 0 and 
100 milliseconds before repeating the number-generating cycle. The Consumer consumes all integers from 
the same file (the exact same file into which the Producer put the integers in the first place) as quickly as they 
become available. The activities of the Producer and the Consumer must be synchronized in a following way: 
The two threads must do some simple coordination. That is, the Producer must have a way to indicate to the 
Consumer that the value is ready, and the Consumer must have a way to indicate that the value has been 
retrieved. Implement the solution using mutex, and binary semaphore — to help threads wait for a condition 
and notify other threads when that condition changes. */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

pthread_mutex_t mutex;
sem_t sem_produce, sem_consume;

void* producer(void* arg) {
    while (1) {
        int num = rand() % 10; // generate random number between 0-9
        usleep((rand() % 100) * 1000); // sleep 0-100 ms

        sem_wait(&sem_produce); // wait for permission to produce
        pthread_mutex_lock(&mutex);

        FILE* f = fopen("shared.txt", "w");
        if (f == NULL) {
            perror("Error opening file");
            exit(1);
        }

        fprintf(f, "%d", num);
        fclose(f);
        printf("Produced: %d\n", num);

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_consume); // signal consumer that data is ready
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        sem_wait(&sem_consume); // wait for data to be ready
        pthread_mutex_lock(&mutex);

        FILE* f = fopen("shared.txt", "r");
        if (f == NULL) {
            perror("Error opening file");
            exit(1);
        }

        int num;
        fscanf(f, "%d", &num);
        fclose(f);

        printf("Consumed: %d\n", num);

        pthread_mutex_unlock(&mutex);
        sem_post(&sem_produce); // signal producer that data is consumed
    }
    return NULL;
}

int main() {
    srand(time(NULL)); // is it necessary here? Yes, to seed the random number generator for producer sleep time.

    pthread_t prod_thread, cons_thread;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&sem_produce, 0, 1); // producer can produce first
    sem_init(&sem_consume, 0, 0); // consumer must wait

    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&sem_produce);
    sem_destroy(&sem_consume);

    return 0;
}
