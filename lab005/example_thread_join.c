/*######################################
# University of Information Technology #
# IT007 Operating System
#
# <Your name>, <your Student ID> #
# File: example_thread_join.c
#
######################################*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define NUM_THREADS 2
void *thread_print(void *threadid)
{
    long tid;
    tid = (long)threadid;
    printf("Hello IT007! I’m Thread #%ld ^_^!!!\n", syscall(SYS_gettid));
    sleep(100);
}
int main()
{
    pthread_t threads[NUM_THREADS];
    int check;
    long tID;
    for (tID = 0; tID < NUM_THREADS; tID++)
    {
        printf("I’m Main Thread: create Thread: #%ld\n", syscall(SYS_gettid));
        check = pthread_create(
            &threads[tID],

            NULL,
            thread_print,
            (void *)tID);

        if (check != 0)
        {
            printf("ERROR!!! I’m Main Thread, I can’t create Thread #%ld ", tID);
            exit(-1);
        } // end if
        pthread_join(threads[tID], NULL);
    } // end for
    /* Last thing that main() should do */
    pthread_exit(NULL);
}