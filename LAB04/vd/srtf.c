#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_REMAINING_BURST 3
#define INT_MAX 100000007

typedef struct
{
    int iPID;
    int iArrival, iBurst, iRemainingBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]);
void printProcess(int n, PCB P[]);
void exportGanttChart(int n, PCB P[]);
void exportGanttChartFixed(int id[], int n, int timeline[], int m);
void pushProcess(int *n, PCB P[], PCB Q);
void removeProcess(int *n, int index, PCB P[]);
int swapProcess(PCB *P, PCB *Q);
int partition(PCB P[], int low, int high, int iCriteria);
void quickSort(PCB P[], int low, int high, int iCriteria);
void calculateAWT(int n, PCB P[]);
void calculateATaT(int n, PCB P[]);

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess = 5;

    // Test Case Input
    Input[0] = (PCB){1, 2, 6, 6, -1, -1, -1, -1, -1};
    Input[1] = (PCB){2, 5, 2, 2, -1, -1, -1, -1, -1};
    Input[2] = (PCB){3, 1, 8, 8, -1, -1, -1, -1, -1};
    Input[3] = (PCB){4, 0, 3, 3, -1, -1, -1, -1, -1};
    Input[4] = (PCB){5, 4, 4, 4, -1, -1, -1, -1, -1};

    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    int currentTime = 0;
    PCB runningProcess = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    int ganttChartID[iNumberOfProcess], ganttChartIDNum = 0;
    int ganttChartStartTime[iNumberOfProcess], ganttChartStartTimeNum = 0;
    int ganttChartEndTime[iNumberOfProcess], ganttChartEndTimeNum = 0;

    int rt[iNumberOfProcess];

    // Copy the burst time into rt[]
    for (int i = 0; i < iNumberOfProcess; i++)
        rt[i] = Input[i].iBurst;

    // quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    int shortest = 0, minm = INT_MAX, check = 0, currP = 0;
    while (iTerminated < iNumberOfProcess)
    {
        // Find process with minimum
        // remaining time among the
        // processes that arrives till the
        // current time
        for (int j = 0; j < iNumberOfProcess; j++)
        {
            if ((Input[j].iArrival <= currentTime) && (rt[j] < minm) && rt[j] > 0)
            {
                minm = rt[j];
                shortest = j;
                check = 1; // changed boolean to integer
            }
        }

        if (check == 0)
        {
            currentTime++;
            continue;
        }

        currP = Input[shortest].iPID;

        if (Input[shortest].iStart == -1)
        {
            if (currentTime != 0)
                Input[shortest].iStart = currentTime + 1;
            else
                Input[shortest].iStart = currentTime;
            Input[shortest].iResponse = Input[shortest].iStart - Input[shortest].iArrival;
        }
        rt[shortest]--;
        ganttChartID[ganttChartIDNum++] = Input[shortest].iPID;
        ganttChartStartTime[ganttChartStartTimeNum++] = Input[shortest].iStart;
        minm = rt[shortest];
        if (minm == 0)
            minm = INT_MAX;

        if (rt[shortest] == 0)
        {

            check = 0; // changed boolean to integer

            // Find finish time of current
            // process

            // Calculate waiting time
            Input[shortest].iFinish = currentTime;
            Input[shortest].iTaT = Input[shortest].iFinish - Input[shortest].iArrival + 1;
            Input[shortest].iWaiting = Input[shortest].iTaT - Input[shortest].iBurst;
            pushProcess(&iTerminated, TerminatedArray, Input[shortest]);

            if (Input[shortest].iWaiting < 0)
                Input[shortest].iWaiting = 0;
        }
        // Increment time
        currentTime++;

        // // Push all newly arrived processes into the ready queue
        // while (iRemain > 0 && Input[0].iArrival <= currentTime) {
        //     pushProcess(&iReady, ReadyQueue, Input[0]);
        //     removeProcess(&iRemain, 0, Input);
        // }

        // If no process is running or the current process has been preempted, select a new process
        // if (iReady > 0 && (runningProcess.iRemainingBurst <= 0 || runningProcess.iRemainingBurst == -1)) {
        //     quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_REMAINING_BURST);
        //     runningProcess = ReadyQueue[0];
        //     if (runningProcess.iStart == -1) {
        //         runningProcess.iStart = currentTime;
        //         runningProcess.iResponse = runningProcess.iStart - runningProcess.iArrival;
        //     }
        //     removeProcess(&iReady, 0, ReadyQueue);
        // }

        // // Execute the running process for one time unit
        // if (runningProcess.iRemainingBurst > 0) {
        //     runningProcess.iRemainingBurst--;
        //     currentTime++;
        // }

        // // If the process has completed, move it to the terminated array
        // if (runningProcess.iRemainingBurst == 0) {
        //     runningProcess.iFinish = currentTime;
        //     runningProcess.iTaT = runningProcess.iFinish - runningProcess.iArrival;
        //     runningProcess.iWaiting = runningProcess.iTaT - runningProcess.iBurst;
        //     pushProcess(&iTerminated, TerminatedArray, runningProcess);
        //     runningProcess.iRemainingBurst = -1; // Reset running process
        // }

        // // If no process is in the ready queue, update current time to the next process arrival
        // if (iReady == 0 && iRemain > 0) {
        //     currentTime = Input[0].iArrival;
        // }
    }

    printf("\n===== SRTF Scheduling =====\n");
    // exportGanttChart(iTerminated, TerminatedArray);
    exportGanttChartFixed(ganttChartID, ganttChartIDNum, ganttChartStartTime, ganttChartStartTimeNum);
    quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    printProcess(iTerminated, TerminatedArray);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);

    return 0;
}

void inputProcess(int n, PCB P[])
{
    for (int i = 0; i < n; ++i)
    {
        printf("Enter details of Process #%d:\n", i + 1);
        P[i].iPID = i + 1;
        printf("Arrival Time: ");
        scanf("%d", &P[i].iArrival);
        printf("Burst Time: ");
        scanf("%d", &P[i].iBurst);
        P[i].iRemainingBurst = P[i].iBurst;
        P[i].iStart = P[i].iFinish = P[i].iWaiting = P[i].iResponse = P[i].iTaT = -1;
    }
}

void printProcess(int n, PCB P[])
{
    printf("\n%-5s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "PID", "Arrival", "Burst", "Start", "Finish", "Waiting", "Response", "TAT");
    for (int i = 0; i < n; ++i)
    {
        printf("%-5d %-10d %-10d %-10d %-10d %-10d %-10d %-10d\n",
               P[i].iPID, P[i].iArrival, P[i].iBurst,
               P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}

void exportGanttChartFixed(int id[], int n, int timeline[], int m)
{
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("  P%d  ", id[i]);
        if (i < n - 1)
            printf("|");
    }
    printf("\n");

    int start = timeline[0];
    printf("%d", start);
    for (int i = 0; i < m; ++i)
    {
        start = timeline[i];
        printf("     %d", start);
    }
    printf("\n");
}

void exportGanttChart(int n, PCB P[])
{
    printf("\nGantt Chart:\n");
    for (int i = 0; i < n; ++i)
    {
        printf("  P%d  ", P[i].iPID);
        if (i < n - 1)
            printf("|");
    }
    printf("\n");

    int start = P[0].iStart;
    printf("%d", start);
    for (int i = 0; i < n; ++i)
    {
        start = P[i].iFinish;
        printf("     %d", start);
    }
    printf("\n");
}

void pushProcess(int *n, PCB P[], PCB Q)
{
    P[*n] = Q;
    (*n)++;
}

void removeProcess(int *n, int index, PCB P[])
{
    for (int i = index; i < *n - 1; ++i)
    {
        P[i] = P[i + 1];
    }
    (*n)--;
}

int swapProcess(PCB *P, PCB *Q)
{
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 1;
}

int partition(PCB P[], int low, int high, int iCriteria)
{
    PCB pivot = P[high];
    int i = low - 1;

    for (int j = low; j < high; ++j)
    {
        int compare;
        switch (iCriteria)
        {
        case SORT_BY_ARRIVAL:
            compare = (P[j].iArrival < pivot.iArrival);
            break;
        case SORT_BY_PID:
            compare = (P[j].iPID < pivot.iPID);
            break;
        case SORT_BY_BURST:
            compare = (P[j].iBurst < pivot.iBurst);
            break;
        case SORT_BY_REMAINING_BURST:
            compare = (P[j].iRemainingBurst < pivot.iRemainingBurst);
            break;
        default:
            compare = 0;
            break;
        }

        if (compare)
        {
            i++;
            swapProcess(&P[i], &P[j]);
        }
    }
    swapProcess(&P[i + 1], &P[high]);
    return (i + 1);
}

void quickSort(PCB P[], int low, int high, int iCriteria)
{
    if (low < high)
    {
        int pi = partition(P, low, high, iCriteria);
        quickSort(P, low, pi - 1, iCriteria);
        quickSort(P, pi + 1, high, iCriteria);
    }
}

void calculateAWT(int n, PCB P[])
{
    int totalWaitingTime = 0;
    for (int i = 0; i < n; ++i)
    {
        totalWaitingTime += P[i].iWaiting;
    }
    float averageWaitingTime = (float)totalWaitingTime / n;
    printf("\nAverage Waiting Time: %.2f\n", averageWaitingTime);
}

void calculateATaT(int n, PCB P[])
{
    int totalTurnaroundTime = 0;
    for (int i = 0; i < n; ++i)
    {
        totalTurnaroundTime += P[i].iTaT;
    }
    float averageTurnaroundTime = (float)totalTurnaroundTime / n;
    printf("Average Turnaround Time: %.2f\n", averageTurnaroundTime);
}
