#include <cs50.h>
#include <stdio.h>

int string_len(string s);

int main(void)
{
    string name = get_string("Name: ");

    printf("%i\n", string_len(name));
}

int string_len(string s)
{
    int n = 0;
    while (s[n] != '\0')
    {
        n++;
    }
}
