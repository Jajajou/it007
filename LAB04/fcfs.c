#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

typedef struct {
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]);
void printProcess(int n, PCB P[]);
void exportGanttChart(int n, PCB P[]);
void pushProcess(int *n, PCB P[], PCB Q);
void removeProcess(int *n, int index, PCB P[]);
int swapProcess(PCB *P, PCB *Q);
int partition(PCB P[], int low, int high, int iCriteria);
void quickSort(PCB P[], int low, int high, int iCriteria);
void calculateAWT(int n, PCB P[]);
void calculateATaT(int n, PCB P[]);

int main() {
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;

    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;

    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);

    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);

    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;

    printf("\nReady Queue: ");
    printProcess(1, ReadyQueue);

    while (iTerminated < iNumberOfProcess) {
        while (iRemain > 0) {
            if (Input[0].iArrival <= ReadyQueue[0].iFinish) {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                continue;
            } else {
                break;
            }
        }

        if (iReady > 0) {
            pushProcess(&iTerminated, TerminatedArray, ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);

            if (iReady > 0) {
                ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
                ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
                ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
                ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
                ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
            }
        }
    }

    printf("\n===== FCFS Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    printProcess(iTerminated, TerminatedArray);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    return 0;
}

void inputProcess(int n, PCB P[]) {
    for (int i = 0; i < n; ++i) {
        printf("Enter details of Process #%d:\n", i + 1);
        P[i].iPID = i + 1;
        printf("Arrival Time: ");
        scanf("%d", &P[i].iArrival);
        printf("Burst Time: ");
        scanf("%d", &P[i].iBurst);
        P[i].iStart = P[i].iFinish = P[i].iWaiting = P[i].iResponse = P[i].iTaT = -1;
    }
}

void printProcess(int n, PCB P[]) {
    printf("\n%-5s %-10s %-10s %-10s %-10s %-10s %-10s\n", "PID", "Arrival", "Burst", "Start", "Finish", "Waiting", "Response");
    for (int i = 0; i < n; ++i) {
        printf("%-5d %-10d %-10d %-10d %-10d %-10d %-10d\n",
               P[i].iPID, P[i].iArrival, P[i].iBurst,
               P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse);
    }
}

void exportGanttChart(int n, PCB P[]) {
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; ++i) {
        printf("  P%d  ", P[i].iPID);
        if (i < n - 1) printf("|");
    }
    printf("\n");

    int start = P[0].iStart;
    printf("%d", start);
    for (int i = 0; i < n; ++i) {
        start = P[i].iFinish;
        printf("     %d", start);
    }
    printf("\n");
}

void pushProcess(int *n, PCB P[], PCB Q) {
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[]) {
    for (int i = index; i < *n - 1; ++i) {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q) {
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 1;
}

int partition(PCB P[], int low, int high, int iCriteria) {
    PCB pivot = P[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        int compare;
        switch (iCriteria) {
            case SORT_BY_ARRIVAL:
                compare = (P[j].iArrival < pivot.iArrival);
                break;
            case SORT_BY_PID:
                compare = (P[j].iPID < pivot.iPID);
                break;
            case SORT_BY_BURST:
                compare = (P[j].iBurst < pivot.iBurst);
                break;
            case SORT_BY_START:
                compare = (P[j].iStart < pivot.iStart);
                break;
            default:
                compare = 0;
                break;
        }

        if (compare) {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria) {
    if (low < high) {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[]) {
    int totalWaitingTime = 0;
    for (int i = 0; i < n; ++i) {
        totalWaitingTime += P[i].iWaiting;
    }
    float averageWaitingTime = (float)totalWaitingTime / n;
    printf("\nAverage Waiting Time: %.2f\n", averageWaitingTime);
}

void calculateATaT(int n, PCB P[]) {
    int totalTurnaroundTime = 0;
    for (int i = 0; i < n; ++i) {
        totalTurnaroundTime += P[i].iTaT;
    }
    float averageTurnaroundTime = (float)totalTurnaroundTime / n;
    printf("Average Turnaround Time: %.2f\n", averageTurnaroundTime);
}
