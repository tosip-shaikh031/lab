#include <stdio.h>
#include <stdlib.h>

// Binary search function
int binarySearch(int arr[], int n, int key) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == key)
            return mid;
        else if (arr[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: ./child_search <sorted_numbers> <key>\n");
        return 1;
    }

    int n = argc - 2; // last argument is key
    int arr[n];

    for (int i = 0; i < n; i++) {
        arr[i] = atoi(argv[i + 1]);
    }

    int key = atoi(argv[argc - 1]);

    printf("\nChild: Received sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\nChild: Searching for %d...\n", key);

    int result = binarySearch(arr, n, key);

    if (result == -1)
        printf("Child: Element %d not found.\n", key);
    else
        printf("Child: Element %d found at position %d.\n", key, result + 1);

    return 0;
}
