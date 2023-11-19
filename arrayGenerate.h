#include <stdlib.h>
#include <time.h>
long *generate(int size)
{
    srand(time(0));
    long *generated = (long *)malloc(sizeof(long) * size);
    for (long i = 0; i < size; i++)
    {
        generated[i] = rand() % 5 + 1;
    }
    return generated;
}
