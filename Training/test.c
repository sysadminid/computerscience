#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int angka = get_int("Mau angka berapa? ");
    int hasil = 1;

    for (int i = 1; i <= angka; i++)
    {
        hasil = hasil * i;
    }
    printf("%i\n", hasil);
}
