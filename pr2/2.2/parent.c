/*2.2 Implement the C program in which main program accepts an integer array. Main program uses the fork 
system call to create a new process called a child process. Parent process sorts an integer array and passes 
the sorted array to child process through the command line arguments of execve system call. The child 
process uses execve system call to load new program that uses this sorted array for performing the binary 
search to search the item in the array.*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Bubble sort function
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

int main() {
    int n;
    printf("Enter number of elements: ");
    scanf("%d", &n);

    int arr[n];
    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Sort the array in parent
    bubbleSort(arr, n);
    printf("Parent: Sorted array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    else if (pid == 0) {
        // Child process
        printf("\nChild process created (PID: %d)\n", getpid());

        // Ask user for element to search
        int key;
        printf("Enter element to search: ");
        scanf("%d", &key);

        // Build argument list for execve
        char *args[n + 3]; // program name + n elements + key + NULL
        args[0] = "./child"; // child program name

        for (int i = 0; i < n; i++) {
            char *num = malloc(10);
            sprintf(num, "%d", arr[i]);
            args[i + 1] = num;
        }

        // Add search key
        char *searchKey = malloc(10);
        sprintf(searchKey, "%d", key);
        args[n + 1] = searchKey;

        // Null terminate the argument list
        args[n + 2] = NULL;

        // Execute child_search program
        execve(args[0], args, NULL);

        // If execve returns, it failed
        perror("execve failed");
        exit(1);
    }

    else {
        // Parent process waits for child
        wait(NULL);
        printf("\nParent: Child process finished.\n");
    }

    return 0;
}
