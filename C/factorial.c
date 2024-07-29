#include <cs50.h>
#include <stdio.h>

int factorial(int n);

int main(void)
{
    int n;

    do
    {
        n = get_int("Enter a number: ");
    }
    while (n < 0);

    printf("%i\n", factorial(n));
}

int factorial(int n)
{
    // Implement factorial function

    // Base case
    if (n == 1)
    {
        return 1;
    }

    return n * factorial(n - 1);
}