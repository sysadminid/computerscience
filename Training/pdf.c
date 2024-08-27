#include <cs50.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    string filename = argv[1];
    FILE *f = fopen(filename, "r");
    u_int8_t buffer[4];
    uint8_t pdf[4] = {37, 80, 68, 70};

    fread(buffer, sizeof(u_int8_t), 4, f);

    for (int i = 0; i < 4; i++)
    {
        printf("%i\n", buffer[i]);
    }

    if (memcmp(buffer, pdf, sizeof(uint8_t)) == 0)
        printf("Checking result: It's pdf file!\n");
    else
        printf("Checking result: It's 'NOT' pdf file.\n");

    fclose(f);
}