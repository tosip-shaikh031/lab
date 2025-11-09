#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

struct shared_data {
    int data_ready;
    char message[SHM_SIZE];
};

int main() {
    key_t key = ftok("server.c", 65);
    int shmid = shmget(key, sizeof(struct shared_data), 0666);
    if (shmid == -1) {
        perror("shmget failed");
        exit(1);
    }

    struct shared_data *shm_ptr = (struct shared_data *)shmat(shmid, NULL, 0);
    if (shm_ptr == (void *)-1) {
        perror("shmat failed");
        exit(1);
    }

    printf("Client: Waiting for server message...\n");

    // Wait until server writes data
    while (shm_ptr->data_ready == 0)
        sleep(1);

    printf("Client: Message received from server: \"%s\"\n", shm_ptr->message);

    // Reset flag to notify server
    shm_ptr->data_ready = 0;

    // Detach shared memory
    shmdt(shm_ptr);

    printf("Client: Done. Exiting.\n");
    return 0;
}
