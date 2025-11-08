/*4.1 The problem below describes two processes, the producer and the consumer, who share a common, 
fixed-size buffer used as a queue. 
The producer is Mr. Simpson whose job is to bake Pizza and consumer is Joey Tribbiani who consumes Pizza 
at the same time. Both of them share common Pizza counter to interact with each other. The problem is to 
make sure that the Simpson won't try to add more pizza on Pizza-counter if it's full. He needs to wait until 
Joey consumes Pizza. Similarly, Joey can't consume pizza from an empty counter. He needs to wait until Mr. 
Simpson adds Pizza on counter. 
Implement a solution in C using POSIX threads that coordinates the activities of the producer Simpson and 
the consumer Joey with counting semaphores and mutex.*/


// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <semaphore.h>
// #include <unistd.h>

// #define BUFFER_SIZE 5

// int pizza_count = 0; // shared counter

// sem_t empty;  // counts empty slots
// sem_t full;   // counts filled slots
// pthread_mutex_t mutex;

// // Producer (Mr. Simpson)
// void* producer(void* arg) {
//     while (1) {
//         sem_wait(&empty);  // Wait if no empty slot available
//         pthread_mutex_lock(&mutex);  // Enter critical section

//         pizza_count++;
//         printf("Simpson baked a pizza! Total pizzas on counter: %d\n", pizza_count);

//         pthread_mutex_unlock(&mutex); // Exit critical section
//         sem_post(&full); // Increase full count

//         sleep(1); // simulate baking time
//     }
// }

// // Consumer (Joey Tribbiani)
// void* consumer(void* arg) {
//     while (1) {
//         sem_wait(&full);   // Wait if no pizza available
//         pthread_mutex_lock(&mutex);  // Enter critical section

//         pizza_count--;
//         printf("Joey ate a pizza! Pizzas left on counter: %d\n", pizza_count);

//         pthread_mutex_unlock(&mutex); // Exit critical section
//         sem_post(&empty); // Increase empty count

//         sleep(2); // simulate eating time
//     }
// }

// int main() {
//     pthread_t prod, cons;

//     // Initialize semaphores and mutex
//     sem_init(&empty, 0, BUFFER_SIZE); // initially all slots are empty
//     sem_init(&full, 0, 0);            // initially no pizzas
//     pthread_mutex_init(&mutex, NULL);

//     // Create threads
//     pthread_create(&prod, NULL, producer, NULL);
//     pthread_create(&cons, NULL, consumer, NULL);

//     // Join threads (though in practice they run forever)
//     pthread_join(prod, NULL);
//     pthread_join(cons, NULL);

//     // Destroy semaphores and mutex (never reached here in this loop)
//     sem_destroy(&empty);
//     sem_destroy(&full);
//     pthread_mutex_destroy(&mutex);

//     return 0;
// }




#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define BUFFER_SIZE 5

sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int pizza_count = 0;

void* producer(void *arg){
    while(1){
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        pizza_count++;
        printf("Simpson Baked the Pizza. Pizza Count :  %d \n",pizza_count);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}
void* consumer(void* arg){
    while(1){
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        pizza_count--;
        printf("Sarish eat the Pizza, PizzaCount : %d \n", pizza_count);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(2);
    }
}


int main(){
    pthread_t cons,prod;

    sem_init(&empty,0,BUFFER_SIZE);
    sem_init(&full,0,0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&prod,NULL,producer,NULL);
    pthread_create(&cons,NULL,consumer,NULL);

    pthread_join(prod,NULL);
    pthread_join(cons,NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}