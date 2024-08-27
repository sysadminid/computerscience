#include <cs50.h>
#include <stdio.h>

int collatz(int n);

int main(void)
{
    int n;

    do
        n = get_int("Enter a number: ");
    while (n < 1);
    
    printf("%i\n", collatz(n));
}

int collatz(int n)
{
    // Implement Collatz conjecture
    if (n == 1)
        return 0;
    else if ((n % 2) == 0)
        return 1 + collatz(n / 2);
    else
        return 1 + collatz(3 * n + 1);
}
