#include <cs50.h>
#include <stdio.h>

int main (void)
{
    string name = get_string("What's your name? ");
    int age = get_int("What's your age? ");
    string phoneNumber = get_string("What's your phone number? +");

    printf("Name: %s\n", name);
    printf("Age: %i\n", age);
    printf("Phone Number: +%s\n", phoneNumber);
}