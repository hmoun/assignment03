#include "arrayGenerate.h"
#include <stdio.h>
#include <pthread.h>
int count;
long *counts;
int finalCount;
int length = 10;
long *ar;
int threadNumber = 32;
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
    counts[id] = count;
}
int main()
{
    count = 0;
    counts = (long *)malloc(sizeof(long) * threadNumber);
    ar = generate(length);
    clock_t start, end;
    double timeTaken;
    int r = 0;
    int trueCount = actualCount();
    start = clock();
    for (int i = 0; i < 100; i++)
    {
        int finalCount=0;
        pthread_t thread[threadNumber];
        for (int i = 0; i < threadNumber; i++)
        {
            pthread_create(&thread[i], NULL, &raceCount, (void *)i);
        }
        for (int i = 0; i < threadNumber; i++)
        {
            pthread_join(thread[i], NULL);
        }
        for (int j = 0; j < threadNumber; j++)
        {
            finalCount += counts[j];
        }
        int p = finalCount;
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
    free(counts);
    return 0;
}
