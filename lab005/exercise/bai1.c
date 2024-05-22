#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>

sem_t sem1, sem2;
int sells = 0, products = 0;

void *processA()
{
    while (1)
    {
        sem_wait(&sem1);
        ++sells;
        printf("sells = %d\n", sells);
        // sleep(2);
        sem_post(&sem2);
    }
}

void *processB()
{
    while (1)
    {
        sem_wait(&sem2);
        ++products;
        printf("products = %d\n", products);
        // sleep(1);
        sem_post(&sem1);
    }
}

void main()
{
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 805);
    pthread_t th1, th2;
    pthread_create(&th1, NULL, &processA, NULL);
    pthread_create(&th2, NULL, &processB, NULL);
    while (1)
        ;
}