#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024  // size of shared memory

struct shared_data {
    int data_ready;          // synchronization flag
    char message[SHM_SIZE];  // message content
};

int main() {
    key_t key = ftok("server.c", 65); // generate unique key
    int shmid = shmget(key, sizeof(struct shared_data), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    struct shared_data *shm_ptr = (struct shared_data *)shmat(shmid, NULL, 0); // create shared memory segment
    if (shm_ptr == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    // Initialize flag
    shm_ptr->data_ready = 0; // data not ready

    printf("Server: Shared memory created.\n");
    printf("Enter message to send to client: ");
    fgets(shm_ptr->message, SHM_SIZE, stdin);
    shm_ptr->message[strcspn(shm_ptr->message, "\n")] = '\0';

    // Simulate processing
    printf("Server: Writing data to shared memory...\n");
    sleep(2);  // simulate delay

    // Set flag
    shm_ptr->data_ready = 1; // data ready
    printf("Server: Message written and flag set.\n");

    printf("Server: Waiting for client to read data...\n");
    while (shm_ptr->data_ready == 1)
        sleep(1);  // wait until client resets flag

    printf("Server: Client has read the message. Cleaning up...\n");

    // Detach and remove shared memory
    shmdt(shm_ptr); // detach shared memory segment
    shmctl(shmid, IPC_RMID, NULL); // remove shared memory segment

    printf("Server: Shared memory removed. Exiting.\n");
    return 0;
}
