#include <cs50.h>
#include <stdio.h>

bool valid_triangle(float x, float y, float z);

int main(void)
{
    float first_side = get_float("Enter first side: ");
    float second_side = get_float("Enter second side: ");
    float height = get_float("Enter height: ");
    printf("Your triangle is %d\n", valid_triangle(first_side, second_side, height));
}

bool valid_triangle(float x, float y, float z)
{
    if (x < 1 || y < 1 || z < 1)
    {
        return false;
    }
    else if ((x + y <= z) || (x + z <= y) || (y + z <= x))
    {
        return false;
    }
    else
    {
        return true;
    }
}