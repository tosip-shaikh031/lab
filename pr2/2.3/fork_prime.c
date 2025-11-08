/*2.3 Write a C program using the fork() system call that generates the prime number sequence in the child 
process. The number will be provided from the command line. For example, if 10 is passed as a parameter 
on the command line, the child process will output 2, 3, 5, 7, 11, 13, 17, 19, 23, 29. Because the parent and 
child processes have their own copies of the data, it will be necessary for the child to output the sequence. 
Have the parent invoke the wait() call to wait for the child process to complete before exiting the program. 
Perform necessary error checking to ensure that a positive integer is passed on the command line.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

// Function to check if a number is prime
bool isPrime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    // Step 1: Validate command-line argument
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive_integer>\n", argv[0]);
        exit(1);
    }

    int n = atoi(argv[1]);  // Convert string to integer

    if (n <= 0) {
        printf("Error: Please enter a positive integer.\n");
        exit(1);
    }

    // Step 2: Create a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    // Step 3: Child process - generates prime numbers
    else if (pid == 0) {
        printf("Child Process (PID: %d) generating first %d prime numbers:\n", getpid(), n);
        int count = 0, num = 2;

        while (count < n) {
            if (isPrime(num)) {
                printf("%d ", num);
                count++;
            }
            num++;
        }
        printf("\nChild Process completed.\n");
        exit(0); // Child exits after printing primes
    }

    // Step 4: Parent process waits for child
    else {
        printf("Parent Process (PID: %d) waiting for child to finish...\n", getpid());
        wait(NULL);
        printf("Parent Process: Child has finished execution.\n");
    }

    return 0;
}


/*
$ gcc fork_prime.c -o fork_prime
$ ./fork_prime 10
Parent Process (PID: 1321) waiting for child to finish...
Child Process (PID: 1322) generating first 10 prime numbers:
2 3 5 7 11 13 17 19 23 29 
Child Process completed.
Parent Process: Child has finished execution.
*/