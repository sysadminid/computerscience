#include <cs50.h>
#include <stdio.h>

int main(int argc, string argv[])
{
    if (argc == 1)
    {
        printf("Not enough argument.\n");
        return 1;
    }
    else if (argc != 2)
    {
        printf("Too many arguments.\n");
        return 2;
    }
    
    printf("hello, %s\n", argv[1]);
    return 0;
}

// Use 'echo $?' to show what is the recently most program secretly exit with.