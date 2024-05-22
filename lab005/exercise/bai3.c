#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0;

void *processA()
{
    while (1)
    {
        x += 1;
        if (x == 20)
            x = 0;
        printf("A: %d\n", x);
    }
}

void *processB()
{
    while (1)
    {
        x += 1;
        if (x == 20)
            x = 0;
        printf("B: %d\n", x);
    }
}

int main()
{
    pthread_t th1, th2;
    pthread_create(&th1, NULL, processA, NULL);
    pthread_create(&th2, NULL, processB, NULL);
    while (1)
        ;
    return 0;
}