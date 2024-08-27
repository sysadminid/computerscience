#include <cs50.h>
#include <math.h>
#include <stdio.h>

int main(void)
{
    int digit, num_len, valid;
    long num;

    // Prompt user input
    num = get_long("Numbers: ");

    // Check for 'num' length
    num_len = floor(log10(num)) + 1;

    // Calculate checksum
    long counter = 10;
    int result = 0;
    int side1 = 0;
    int side2 = num % 10;

    for (int i = 0; i < (num_len - 1); i += 2)
    {
        side1 = ((num / counter) % 10) * 2;
        side2 += ((num / (counter * 10)) % 10);

        // check if 'checksum' has 2 digits
        digit = floor(log10(side1)) + 1;
        ;
        if (digit > 1)
        {
            side1 = (side1 % 10) + ((side1 / 10) % 10);
        }

        result += side1;
        counter *= 100;
    }
    // check validity
    valid = (result += side2) % 10;

    // Check for starting digits
    int start_digit = num / pow(10, (num_len - 2));

    if (valid == 0)
    {
        if ((start_digit == 34 || start_digit == 37) && num_len == 15)
        {
            printf("AMEX\n");
        }
        else if (start_digit > 50 && start_digit < 56 && num_len == 16)
        {
            printf("MASTERCARD\n");
        }
        else if ((start_digit / 10) == 4 && (num_len == 13 || num_len == 16))
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }
}