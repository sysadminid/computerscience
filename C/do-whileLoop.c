#include <stdio.h>

int main(void)
{
    int x = 0;

    do
    {
        printf("%i\n", (x + 1));
        x++;
    }
    while (x < 3);
}