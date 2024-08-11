#include <stdio.h>

int main(void) {
    int n = 50;
    int *p = &n;

    // Show me pointer address of n
    printf("%p\n", &n);

    // Show me what's inside this pointer *p
    printf("%i\n", *p);
}