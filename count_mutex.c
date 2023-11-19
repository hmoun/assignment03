#include "arrayGenerate.h"
#include <stdio.h>
#include <pthread.h>
int count = 0;
long length = 1000000;
long *ar;
int threadNumber = 4;
pthread_mutex_t mutex;
int actualCount()
{
    int i = 0;
    int trueCount = 0;
    for (i = 0; i < length; i++)
    {
        if (ar[i] == 1)
        {
            trueCount++;
        }
    }
    return trueCount;
}
void *raceCount(void *ID)
{
    long id = (long)ID;
    long iterations = length / threadNumber;
    long start = id * iterations;
    long end = start + iterations;
    if (length - end < iterations)
    {
        end = length;
    }
    for (start; start < end; start++)
    {
        if (ar[start] == 1)
        {
            pthread_mutex_lock(&mutex);
            count++;
            pthread_mutex_unlock(&mutex);
        }
    }
}
int main()
{
    pthread_mutex_init(&mutex, NULL);
    ar = generate(length);
    clock_t start, end;
    double timeTaken;
    int r;
    int trueCount = actualCount();
    start = clock();
    for (int i = 0; i < 100; i++)
    {
        count = 0;
        pthread_t thread[threadNumber];
        for (int i = 0; i < threadNumber; i++)
        {
            pthread_create(&thread[i], NULL, &raceCount, (void *)i);
        }
        for (int i = 0; i < threadNumber; i++)
        {
            pthread_join(thread[i], NULL);
        }
        int p = count;
        if (p == trueCount)
        {
            r++;
        }
    }
    printf("%d", r);
    end = clock();
    timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n%f secs", timeTaken);
    free(ar);
    pthread_mutex_destroy(&mutex);
    return 0;
}
