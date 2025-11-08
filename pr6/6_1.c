/*6.1 Write a C program to simulate page replacement algorithms a) FIFO b) LRU c)Optimal
First, generate a random page-reference string where page numbers range from 0 to 9. Apply the random 
page-reference string to each algorithm and record the number of page faults incurred by each algorithm. 
Implement the replacement algorithms so that the number of page frames can vary from 1 to 7.*/

/* 6.2  Consider the page reference string of size 12: 1, 2, 3, 4, 1, 2, 5, 1, 1, 2, 3, 4, 5 with frame size 3 and 4 
respectively. Write a C program to simulate page replacement algorithms a) Optimal b) LRU.*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PAGES 20
#define MAX_FRAMES 7

// Function to generate random page reference string
void generateReferenceString(int pages[], int n) {
    for (int i = 0; i < n; i++)
        pages[i] = rand() % 10;  // page numbers from 0 to 9
}


// Function to print reference string
void printReferenceString(int pages[], int n) {
    printf("Page Reference String: ");
    for (int i = 0; i < n; i++)
        printf("%d ", pages[i]);
    printf("\n");
}


// Function to check if a page is in frames
int isHit(int frames[], int n, int page) {
    for (int i = 0; i < n; i++)
        if (frames[i] == page)
            return 1;
    return 0;
}



int main() {
    srand(time(NULL));
    
    int pages[MAX_PAGES];
    int n = MAX_PAGES;
    generateReferenceString(pages, n);
    printReferenceString(pages, n);

    printf("\nFrame Size\tFIFO\tLRU\tOptimal\n");
    printf("----------------------------------\n");
    
    for (int frames = 1; frames <= 7; frames++) {
        int fifoFaults = FIFO(pages, n, frames);
        int lruFaults = LRU(pages, n, frames);
        int optFaults = Optimal(pages, n, frames);
        
        printf("%d\t\t%d\t%d\t%d\n", frames, fifoFaults, lruFaults, optFaults);
    }

    return 0;
}

// FIFO Page Replacement
int FIFO(int pages[], int n, int frameCount) {
    int frames[MAX_FRAMES], index = 0, pageFaults = 0;

    for (int i = 0; i < frameCount; i++)
        frames[i] = -1; // initialize as empty

    for (int i = 0; i < n; i++) {
        if (!isHit(frames, frameCount, pages[i])) {
            frames[index] = pages[i];
            index = (index + 1) % frameCount; // circular queue behavior
            pageFaults++;
        }
    }

    return pageFaults;
}

// LRU Page Replacement
int LRU(int pages[], int n, int frameCount) {
    int frames[MAX_FRAMES], recent[MAX_FRAMES], time = 0, pageFaults = 0;

    for (int i = 0; i < frameCount; i++) {
        frames[i] = -1;
        recent[i] = 0;
    }

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        int hit = 0;
        //check if page exists in frame
        for (int j = 0; j < frameCount; j++) {
            if (frames[j] == page) {
                hit = 1;
                recent[j] = ++time;
                break;
            }
        }
        //if it doesnt then replace LRU page
        if (!hit) {
            int lruIndex = 0;
            for (int j = 1; j < frameCount; j++)
                if (recent[j] < recent[lruIndex])
                    lruIndex = j;

            frames[lruIndex] = page;
            recent[lruIndex] = ++time;
            pageFaults++;
        }
    }

    return pageFaults;
}

// Optimal Page Replacement
int Optimal(int pages[], int n, int frameCount) {
    int frames[MAX_FRAMES], pageFaults = 0;

    for (int i = 0; i < frameCount; i++)
        frames[i] = -1;

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        //if found continue
        if (isHit(frames, frameCount, page))
            continue;
                
        int replaceIndex = -1, farthest = i + 1;
        //loop through frame to find the optimal page to replcar
        for (int j = 0; j < frameCount; j++) {
            //if frame position is empty then replace it 
            if (frames[j] == -1) {
                replaceIndex = j;
                break;
            }
            //loop through the reference string to find its first future use
            int nextUse = -1;
            for (int k = i + 1; k < n; k++) {
                if (frames[j] == pages[k]) {
                    nextUse = k;
                    break;
                }
            }
            //if not used in future remove
            if (nextUse == -1) {
                replaceIndex = j;
                break;
            }
            //update farthest
            if (nextUse > farthest) {
                farthest = nextUse;
                replaceIndex = j;
            }
        }
                
        frames[replaceIndex] = page;
        pageFaults++;
    }

    return pageFaults;
}
