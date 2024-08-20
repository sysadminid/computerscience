#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string s = get_string("Input: ");

    printf("Output: %lu\n", strlen(s));
}
