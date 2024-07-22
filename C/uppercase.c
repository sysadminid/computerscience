#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("Before: ");
    printf("After (toupper): ");
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        printf("%c", toupper(s[i]));
    }
    printf("\n");

    printf("After (manipulate memory): ");
    for (int j = 0, o = strlen(s); j < o; j++)
    {
        // if lowercase
        if (s[j] >= 'a' && s[j] <= 'z')
        {
            printf("%c", s[j] - 32);
        }
        else
        {
            printf("%c", s[j]);
        }
    }
    printf("\n");

    printf("After (manipulate memory with clever way): ");
    for (int k = 0, p = strlen(s); k < p; k++)
    {
        // if lowercase
        if (s[k] >= 'a' && s[k] <= 'z')
        {
            printf("%c", s[k] - ('a' - 'A'));
        }
        else
        {
            printf("%c", s[k]);
        }
    }
    printf("\n");
}
