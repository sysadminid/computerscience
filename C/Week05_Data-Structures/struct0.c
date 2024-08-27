#include <stdio.h>
#include <stdlib.h>

struct car
{
    int year;
    char *model[10];
};

int main(void)
{
    struct car *mycar = malloc(sizeof(struct car));

    mycar->year = 2024;
    *mycar->model = "Tesla";

    printf("%s\n", *mycar->model);
}

