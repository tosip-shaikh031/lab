#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CHAIRS 3   // number of waiting chairs in the hallway
#define MAX_STUDENTS 10 // number of students

sem_t students_sem;     // counts number of students waiting
sem_t ta_sem;           // for TA to signal readiness to help a student
pthread_mutex_t mutex;  // mutex to protect waiting_students variable

int waiting_students = 0; // number of students currently waiting

// ---------------- TA Thread ----------------
// like producer
void *ta_work(void *arg) {
    while (1) {
        // Wait until a student arrives (if no student, TA sleeps)
        sem_wait(&students_sem); // Wait for a student to arrive

        // A student is ready, now try to acquire lock and help them
        pthread_mutex_lock(&mutex);
        waiting_students--;
        printf("TA starts helping a student. Students waiting: %d\n", waiting_students);
        pthread_mutex_unlock(&mutex);

        // Signal that TA is now busy helping a student
        sem_post(&ta_sem);

        // Simulate time taken to help the student
        sleep(rand() % 3 + 1);

        printf("TA finished helping a student and checks for next one.\n");
    }
    return NULL;
}



// ---------------- Student Thread ----------------
// like consumer
void *student_work(void *arg) {
    int id = *(int *)arg; // student ID

    while (1) {
        printf("Student %d is programming...\n", id);
        sleep(rand() % 5 + 1); // simulate time before needing help

        pthread_mutex_lock(&mutex);
        if (waiting_students < MAX_CHAIRS) {
            waiting_students++;
            printf("Student %d waiting. Students waiting: %d\n", id, waiting_students);

            // Notify TA that a student is waiting
            sem_post(&students_sem);
            pthread_mutex_unlock(&mutex);

            // Wait for TA to help
            sem_wait(&ta_sem);
            printf("Student %d is getting help from the TA.\n", id);
        } else {
            pthread_mutex_unlock(&mutex);
            printf("Student %d found no empty chair. Will come back later.\n", id);
        }
    }
    return NULL;
}


// ---------------- Main ----------------
int main() {
    pthread_t ta;
    pthread_t students[MAX_STUDENTS]; // array to hold student threads
    int student_ids[MAX_STUDENTS];

    srand(time(NULL)); // seed for random number generation

    // Initialize synchronization primitives
    sem_init(&students_sem, 0, 0);  // initially no students waiting
    sem_init(&ta_sem, 0, 0);        // TA is sleeping initially
    pthread_mutex_init(&mutex, NULL);

    // Create TA thread
    pthread_create(&ta, NULL, ta_work, NULL);

    // Create student threads
    for (int i = 0; i < MAX_STUDENTS; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_work, &student_ids[i]);
    }

    // Join threads (infinite loop, so not actually reached)
    pthread_join(ta, NULL);
    for (int i = 0; i < MAX_STUDENTS; i++)
        pthread_join(students[i], NULL);

    // Cleanup (never reached here)
    sem_destroy(&students_sem);
    sem_destroy(&ta_sem);
    pthread_mutex_destroy(&mutex);

    return 0;
}
