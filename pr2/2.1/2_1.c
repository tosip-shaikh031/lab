/* Implement the C program to accept ‘n’ integers to be sorted. Main function creates child process using 
fork system call. Parent process sorts the integers using bubble sort and waits for child process using wait 
system call. Child process sorts the integers using insertion sort. Also demonstrate zombie and orphan states. */

//Orphan
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Function to print array
void printArray(int arr[], int n, const char *msg) {
    printf("%s: ", msg);
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Bubble Sort (Parent)
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

// Insertion Sort (Child)
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

    printf("Enter number of integers: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    pid_t pid = fork();

    if (pid < 0) {
        printf("Fork failed!\n");
        return 1;
    }

    else if (pid == 0) {
        // Child process
        printf("\n[Child] Process ID: %d | Parent ID: %d\n", getpid(), getppid());
        printf("[Child] Performing Insertion Sort...\n");

        insertionSort(arr, n);
        printArray(arr, n, "[Child] Sorted array (Insertion Sort)");

        printf("\n[Child] Sleeping for 5 seconds to demonstrate ORPHAN state...\n");
        sleep(5);
        printf("[Child] After sleep, Parent ID is now %d (adopted by init/systemd)\n", getppid());
        printf("[Child] Exiting...\n");
    }

    else {
        // Parent process
        printf("\n[Parent] Process ID: %d | Child ID: %d\n", getpid(), pid);
        printf("[Parent] Performing Bubble Sort...\n");

        bubbleSort(arr, n);
        printArray(arr, n, "[Parent] Sorted array (Bubble Sort)");

        printf("\n[Parent] Exiting immediately to create orphan child...\n");
        exit(0);  // Parent exits before child finishes
    }

    return 0;
}


//Zombie
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/wait.h>

// // Function to print array
// void printArray(int arr[], int n, const char *msg) {
//     printf("%s: ", msg);
//     for (int i = 0; i < n; i++) {
//         printf("%d ", arr[i]);
//     }
//     printf("\n");
// }

// // Bubble Sort (Parent)
// void bubbleSort(int arr[], int n) {
//     for (int i = 0; i < n - 1; i++) {
//         for (int j = 0; j < n - i - 1; j++) {
//             if (arr[j] > arr[j + 1]) {
//                 int temp = arr[j];
//                 arr[j] = arr[j + 1];
//                 arr[j + 1] = temp;
//             }
//         }
//     }
// }

// // Insertion Sort (Child)
// void insertionSort(int arr[], int n) {
//     for (int i = 1; i < n; i++) {
//         int key = arr[i];
//         int j = i - 1;
//         while (j >= 0 && arr[j] > key) {
//             arr[j + 1] = arr[j];
//             j--;
//         }
//         arr[j + 1] = key;
//     }
// }

// int main() {
//     int n;

//     printf("Enter number of integers: ");
//     scanf("%d", &n);

//     int arr[n];
//     printf("Enter %d integers: ", n);
//     for (int i = 0; i < n; i++) {
//         scanf("%d", &arr[i]);
//     }

//     pid_t pid = fork();

//     if (pid < 0) {
//         printf("Fork failed!\n");
//         return 1;
//     }

//     else if (pid == 0) {
//         // Child process
//         printf("\n[Child] Process ID: %d | Parent ID: %d\n", getpid(), getppid());
//         printf("[Child] Performing Insertion Sort...\n");

//         insertionSort(arr, n);
//         printArray(arr, n, "[Child] Sorted array (Insertion Sort)");

//         printf("[Child] Exiting now...\n");
//         exit(0); // Child finishes before parent waits → zombie
//     }

//     else {
//         // Parent process
//         printf("\n[Parent] Process ID: %d | Child ID: %d\n", getpid(), pid);
//         printf("[Parent] Performing Bubble Sort...\n");

//         bubbleSort(arr, n);
//         printArray(arr, n, "[Parent] Sorted array (Bubble Sort)");

//         printf("\n[Parent] Sleeping for 10 seconds to demonstrate ZOMBIE state...\n");
//         sleep(10);  // Child has exited, parent still alive → zombie state
//         printf("[Parent] Woke up, now calling wait()...\n");

//         wait(NULL);
//         printf("[Parent] Child process has been reaped. No more zombies.\n");
//     }

//     return 0;
// }
