// Implements a phone book with structs

#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    string name;
    string number;
}
person;

int main(void)
{
    person people[3];

    people[0].name = "Dita";
    people[0].number = "+62-123-456-1000";

    people[1].name = "Sofyan";
    people[1].number = "+62-123-456-2000";

    people[2].name = "Aqsa";
    people[2].number = "+62-123-456-3000";

    // Search for name
    string name = get_string("Name: ");
    for (int i = 0; i < 3; i++)
    {
        if (strcmp(people[i].name, name) == 0)
        {
            printf("Found %s\n", people[i].number);
            return 0;
        }
    }
    printf("Not found\n");
    return 1;
}