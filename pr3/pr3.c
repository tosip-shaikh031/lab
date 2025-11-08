/*Write a menu-driven C program to simulate the following CPU scheduling algorithms to find average 
turnaround time and average waiting time. 
a) FCFS b) SJF (preemptive) c) SJF(non preeemptive) d) RR

Enter the number of processes:3 
 Process  Arrival time  Burst time 
 p1  0   7 
 p2  2   4 
 p3  4   1 
 p4  5   4 
 
Enter the choice: 1. FCFS 2. SJF (preemptive)  3. Exit 
Average waiting time for <FCFS/SJF> is: 
Average turn-around time for <FCFS/SJF> is: 
GANTT CHART: 
_________________ 
| p* | p* | p* |  
_________________ 
0      5      9      12 

*/



#include <stdio.h>
#include <string.h>

struct Process {
    char name[5];
    int at, bt, ct, tat, wt;
    int remaining;  // For preemptive algorithms
};


// Function Prototypes
void sortByArrival(struct Process p[], int n);
void fcfs(struct Process p[], int n);
void sjfNonPreemptive(struct Process p[], int n);
void sjfPreemptive(struct Process p[], int n);
void roundRobin(struct Process p[], int n, int tq);
void displayResults(struct Process p[], int n, float avgTAT, float avgWT);
void printGanttChart(char order[][5], int timePoints[], int count);

// Sort processes by arrival time
void sortByArrival(struct Process p[], int n) {
    struct Process temp;
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].at > p[j].at) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
}

// FCFS Scheduling
void fcfs(struct Process p[], int n) {
    sortByArrival(p, n);
    int time = 0;
    float totalTAT = 0, totalWT = 0;
    char order[100][5];
    int timePoints[100], count = 0;

    for (int i = 0; i < n; i++) {
        if (time < p[i].at) time = p[i].at;
        strcpy(order[count], p[i].name);
        timePoints[count] = time;
        count++;
        time += p[i].bt;
        p[i].ct = time;
        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
        totalTAT += p[i].tat;
        totalWT += p[i].wt;
    }
    timePoints[count] = time;
    displayResults(p, n, totalTAT / n, totalWT / n);
    printGanttChart(order, timePoints, count);
}

void fcfs(struct Process p[], int n){
    int time = 0;
    int count = 0;
    int totalWt = 0;
    int totalTAT = 0;
    char order[100][5];
    int timeFrame[100];

    for(int i = 0;i<n;i++){
        if(p[i].at > time){
            time = p[i].at;
        }
        strcpy(order[count],p[i].name);
        

    }
}

// SJF Non-Preemptive
void sjfNonPreemptive(struct Process p[], int n) {
    sortByArrival(p, n);
    int completed = 0, time = 0, done[20] = {0};
    float totalTAT = 0, totalWT = 0;
    char order[100][5];
    int timePoints[100], count = 0;

    while (completed < n) {
        int idx = -1, minBT = 9999;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].at <= time && p[i].bt < minBT) {
                minBT = p[i].bt;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        strcpy(order[count], p[idx].name);
        timePoints[count++] = time;
        time += p[idx].bt;
        p[idx].ct = time;
        p[idx].tat = p[idx].ct - p[idx].at;
        p[idx].wt = p[idx].tat - p[idx].bt;
        done[idx] = 1;
        completed++;
        totalTAT += p[idx].tat;
        totalWT += p[idx].wt;
    }
    timePoints[count] = time;
    displayResults(p, n, totalTAT / n, totalWT / n);
    printGanttChart(order, timePoints, count);
}

// SJF Preemptive (Shortest Remaining Time First)
void sjfPreemptive(struct Process p[], int n) {
    sortByArrival(p, n);
    for (int i = 0; i < n; i++) p[i].remaining = p[i].bt;

    int completed = 0, time = 0;
    float totalTAT = 0, totalWT = 0;
    char order[200][5];
    int timePoints[200], count = 0;
    int prev = -1;

    while (completed < n) {
        int idx = -1, minRem = 9999;
        for (int i = 0; i < n; i++) {
            if (p[i].at <= time && p[i].remaining > 0 && p[i].remaining < minRem) {
                minRem = p[i].remaining;
                idx = i;
            }
        }

        if (idx == -1) {
            time++;
            continue;
        }

        // Log Gantt Chart entry when process changes
        if (idx != prev) {
            strcpy(order[count], p[idx].name);
            timePoints[count++] = time;
            prev = idx;
        }

        p[idx].remaining--;
        time++;

        if (p[idx].remaining == 0) {
            p[idx].ct = time;
            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;
            totalTAT += p[idx].tat;
            totalWT += p[idx].wt;
            completed++;
        }
    }
    timePoints[count] = time;
    displayResults(p, n, totalTAT / n, totalWT / n);
    printGanttChart(order, timePoints, count);
}

// Round Robin
void roundRobin(struct Process p[], int n, int tq) {
    sortByArrival(p, n);
    for (int i = 0; i < n; i++) p[i].remaining = p[i].bt;

    int time = 0, completed = 0;
    float totalTAT = 0, totalWT = 0;
    char order[200][5];
    int timePoints[200], count = 0;

    while (completed < n) {
        int doneSomething = 0;
        for (int i = 0; i < n; i++) {
            if (p[i].remaining > 0 && p[i].at <= time) {
                doneSomething = 1;
                strcpy(order[count], p[i].name);
                timePoints[count++] = time;

                if (p[i].remaining > tq) {
                    time += tq;
                    p[i].remaining -= tq;
                } else {
                    time += p[i].remaining;
                    p[i].remaining = 0;
                    p[i].ct = time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    totalTAT += p[i].tat;
                    totalWT += p[i].wt;
                    completed++;
                }
            }
        }
        if (!doneSomething) time++; // CPU idle
    }

    timePoints[count] = time;
    displayResults(p, n, totalTAT / n, totalWT / n);
    printGanttChart(order, timePoints, count);
}

// Display Results
void displayResults(struct Process p[], int n, float avgTAT, float avgWT) {
    printf("\nProcess\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++)
        printf("%s\t%d\t%d\t%d\t%d\t%d\n",
               p[i].name, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);

    printf("\nAverage Turnaround Time = %.2f", avgTAT);
    printf("\nAverage Waiting Time = %.2f\n", avgWT);
}

// Print Gantt Chart
void printGanttChart(char order[][5], int timePoints[], int count) {
    printf("\nGANTT CHART:\n");
    printf("___________________________\n|");
    for (int i = 0; i < count; i++)
        printf(" %s |", order[i]);
    printf("\n---------------------------\n");
    for (int i = 0; i <= count; i++)
        printf("%d\t", timePoints[i]);
    printf("\n");
}

// Main Function
int main() {
    int n, choice, tq;
    struct Process p[20];

    printf("Enter number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nEnter details for process %d:\n", i + 1);
        printf("Process name: ");
        scanf("%s", p[i].name);
        printf("Arrival time: ");
        scanf("%d", &p[i].at);
        printf("Burst time: ");
        scanf("%d", &p[i].bt);
    }

    while (1) {
        printf("\n--- CPU SCHEDULING MENU ---\n");
        printf("1. FCFS\n2. SJF (Non-preemptive)\n3. SJF (Preemptive)\n4. Round Robin\n5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: fcfs(p, n); break;
            case 2: sjfNonPreemptive(p, n); break;
            case 3: sjfPreemptive(p, n); break;
            case 4:
                printf("Enter Time Quantum: ");
                scanf("%d", &tq);
                roundRobin(p, n, tq);
                break;
            case 5: return 0;
            default: printf("Invalid choice!\n");
        }
    }
}
