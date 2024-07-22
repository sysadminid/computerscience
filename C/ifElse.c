#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int x = get_int("Input your 'x' : ");

    if ((x % 2) == 0)
    {
        printf("%i is even number\n", x);
    }
    else
    {
        printf("%i is odd number\n", x);
    }
}