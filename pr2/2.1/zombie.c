#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Bubble Sort function (Parent)
void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Insertion Sort function (Child)
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
        scanf("%d", &arr[i]);

    pid_t pid = fork(); // creaate a new process

    if (pid < 0) {
        printf("Fork failed!\n");
        exit(1);
    }
    else if (pid == 0) {
        // ---- CHILD PROCESS ----
        printf("\n[Child] PID: %d | PPID: %d\n", getpid(), getppid()); // getppid() gets parent process ID
        insertionSort(arr, n);
        printf("[Child] Sorted array using Insertion Sort:\n");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        // To simulate orphan or zombie, we’ll use sleep() later
        // sleep(30); // Uncomment for ORPHAN
    }
    else {
        // ---- PARENT PROCESS ----
        printf("\n[Parent] PID: %d | Child PID: %d\n", getpid(), pid);
        bubbleSort(arr, n);
        printf("[Parent] Sorted array using Bubble Sort:\n");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");

        // Uncomment wait() for normal or zombie demonstration
        // wait(NULL); // Wait for child to finish (no zombie)
        // printf("[Parent] Child completed. Parent exiting.\n");
        // exit(0);
        sleep(100); // Uncomment for ZOMBIE
    }

    return 0;
}


// this command to check
// ps -ef | grep a.out
