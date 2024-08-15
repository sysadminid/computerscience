#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;

    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (int i = 0; i < height; i++)
    {
        // print 'spaces'
        for (int j = 1; height > (j + i); j++)
        {
            printf(" ");
        }

        // print 'left_hashes'
        for (int k = 0; k <= i; k++)
        {
            printf("#");
        }

        // print 'gap'
        printf("  ");

        // print 'right_hashes'
        for (int l = 0; l <= i; l++)
        {
            printf("#");
        }
        printf("\n");
    }
}