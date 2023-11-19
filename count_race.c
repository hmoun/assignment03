#include "arrayGenerate.h"
#include <stdio.h>
#include <pthread.h>
int count = 0;
int length = 1000000000; //for testing purposes
long *ar;
int threadNumber = 32; //for testing purposes
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
    count = 0;
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
            count++;
        }
    }
}
int main()
{
    ar = generate(length);
    clock_t start, end;
    double timeTaken;
    int r;
    int trueCount = actualCount();
    start = clock();
    for (int i = 0; i < 100; i++)
    {
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
    printf("%d %d %d", count, trueCount, r);
    end = clock();
    timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n%f secs", timeTaken);
    free(ar);
    return 0;
}
