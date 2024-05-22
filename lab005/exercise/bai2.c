#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_SIZE 100000

sem_t sem1, sem2;
int n;
int i = 0;
static int dem = 0;
int a[MAX_SIZE];

void *PROCESS1()
{
    while (1)
    {
        sem_wait(&sem2); // Wait for permission to produce
        if (dem < n)
        {
            a[i++] = rand() % (n - 1);
            dem++;
            printf("\nNumber of elements in array a after push: %2d", dem);
        }
        sem_post(&sem1); // Signal that an item has been produced
    }
}

void *PROCESS2()
{
    int j, b;
    while (1)
    {
        sem_wait(&sem1); // Wait for permission to consume
        if (dem == 0)
        {
            printf("\nNothing in array a");
        }
        else
        {
            dem--;
            if (dem == 0)
            {
                printf("\nNothing in array a");
            }
            else
            {
                b = a[0];
                for (j = 0; j < dem; j++)
                {
                    a[j] = a[j + 1];
                }
                printf("\nNumber of elements in array a after pop: %2d", dem);
            }
        }
        sem_post(&sem2); // Signal that an item has been consumed
    }
}

int main()
{
    srand(time(NULL));     // Seed the random number generator
    sem_init(&sem1, 0, 0); // Initialize sem1 to 0

    printf("Enter n: ");
    scanf("%d", &n);
    sem_init(&sem2, 0, n); // Initialize sem2 to n

    pthread_t th1, th2;
    pthread_create(&th1, NULL, PROCESS1, NULL);
    pthread_create(&th2, NULL, PROCESS2, NULL);

    pthread_join(th1, NULL); // Wait for thread 1 to finish
    pthread_join(th2, NULL); // Wait for thread 2 to finish

    return 0;
}
