#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int x = get_int("Choose menu 1-3: ");

    switch (x)
    {
    case 1:
        printf("This is first menu\n");
        break;
    case 2:
        printf("This is second menu\n");
        break;
    case 3:
        printf("This is third menu\n");
        break;
    default:
        printf("No menu for '%i'\n", x);
        break;
    }
}