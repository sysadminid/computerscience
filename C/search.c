#include <cs50.h>
#include <stdio.h>

typedef struct
{
    string name;
    int votes;
} candidate;

int main(void)
{
    const int num_candidates = 3;
    candidate candidates[num_candidates];

    candidates[0].name = "Dita";
    candidates[0].votes = 10;

    candidates[1].name = "Sofyan";
    candidates[1].votes = 7;

    candidates[2].name = "Aliya";
    candidates[2].votes = 12;

    // Find the highest votes (Linear Search)
    int highest_votes = 0;
    for (int i = 0; i < num_candidates; i++)
    {
        if (candidates[i].votes > highest_votes)
        {
            highest_votes = candidates[i].votes;
        }
    }
    printf("%i\n", highest_votes);

    // Find who has highest votes (Linear Search)
    for (int i = 0; i < num_candidates; i++)
    {
        if (candidates[i].votes == highest_votes)
        {
            printf("%s\n", candidates[i].name);
        }
    }
}