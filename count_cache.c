#include "arrayGenerate.h"
#include <stdio.h>
#include <pthread.h>
int count = 0;
int length = 100;
long *ar;
int threadNumber = 32;
long *counts;
int finalCount;
typedef struct cacheOccupation cache;
struct cacheOccupation
{
    long number;
    char *dummy;
};
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
    cache *ptr = (cache *)ID;
    count = 0;
    long iterations = length / threadNumber;
    long start = (ptr->number) * iterations;
    long end = start + iterations;
    int toFill = 32768 - ((iterations * 8) + (3 * sizeof(long)) + sizeof(int));
    ptr->dummy = (char *)malloc(toFill * sizeof(char));
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
    counts[ptr->number] = count;
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
    free(ar);
    free(counts);
    return 0;
}
