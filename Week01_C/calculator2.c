// Helper function with arguments and return value

#include <cs50.h>
#include <stdio.h>

int add(int a, int b);

int main(void)
{
    // Prompt user for x
    int x = get_int("x: ");

    // Prompt user for y
    int y = get_int("y: ");

    // Perform addition
    int z = add(x, y);
    printf("%i\n", z);
}

int add(int a, int b)
{
    int c = a + b;
    return c;
}