#include "arrayGenerate.h"
#include <stdio.h>
#include <pthread.h>
int count;
int length = 1000;
long *generated;
int threadNumber = 10;
long *counts;
int finalCount;
int trueCount = 0;
typedef struct cacheOccupation cache;
struct cacheOccupation
{
    long number;
    char *dummy;
};
int actualCount()
{
    int i = 0;
    for (i = 0; i < length; i++)
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
    count = 0;
    cache *ptr = (cache *)ID;
    long iterations = length / threadNumber;
    long start = (ptr->number) * iterations;
    long end = start + iterations;
    int toFill = 131072 - ((iterations * 8) + (sizeof(long) * 3) + sizeof(int));
    ptr->dummy = (char *)malloc(toFill * sizeof(char));
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
    counts[ptr->number] = count;
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
            cache c;
            c.number = i;
            pthread_create(&thread[i], NULL, &raceCount, &c);
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
