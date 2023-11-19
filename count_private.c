#include "arrayGenerate.h"
#include <stdio.h>
#include <pthread.h>
long *counts;
long length = 10000;
long *generated;
int threadNumber = 32;
int trueCount = 0;
int actualCount()
{
    for (int i = 0; i < length; i++)
    {
        if (generated[i] == 1)
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
    int count = 0;
    if (length - end < iterations)
    {
        end = length;
    }
    for (start; start < end; start++)
    {
        if (generated[start] == 1)
        {
            count++;
        }
    }
    counts[id] = count;
}
int main()
{
    counts = (long *)malloc(sizeof(long) * threadNumber);
    generated = generate(length);
    int trueCount = actualCount();
    clock_t start, end;
    double timeTaken;
    int r = 0;
    start = clock();
    for (int i = 0; i < 100; i++)
    {
        int finalCount = 0;
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
        if (finalCount == trueCount)
        {
            r++;
        }
    }
    printf("%d", r);
    end = clock();
    timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\n%f secs", timeTaken);
    free(generated);
    free(counts);
    return 0;
}
