#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int x = get_int("Input '5' to open the secret: ");
    // ternary operator in C using ?: as short-hand if else
    printf((x == 5) ? "C is fun\n" : "Nope\n");
}