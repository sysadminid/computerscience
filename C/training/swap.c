#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b);

int main(void)
{
    int a = 50;
    int b = 10;

    printf("a is %p, b is %p\n", &a, &b);
    printf("a is %i, b is %i\n\n", a, b);
    swap(&a, &b);
    printf("a is %p, b is %p\n", &a, &b);
    printf("a is %i, b is %i\n", a, b);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}