#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
pair pairs[MAX * (MAX - 1) / 2];
string candidates[MAX];

int candidate_count, pair_count, voter_count;

// Function prototypes
bool cycle_exist(int winner, int loser);
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
int compare_pairs(const void *a, const void *b);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    for (int i = 0; i < candidate_count; i++)
        candidates[i] = argv[i + 1];

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
        for (int j = 0; j < candidate_count; j++)
            locked[i][j] = false;

    pair_count = 0;
    voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int x = 0; x < candidate_count; x++)
        for (int y = x + 1; y < candidate_count; y++)
            preferences[ranks[x]][ranks[y]]++;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

// Compare function is used for qsort function to compare between two items
int compare_pairs(const void *a, const void *b)
{
    pair *pair_a = (pair *) a;
    pair *pair_b = (pair *) b;

    int strength_a = preferences[pair_a->winner][pair_a->loser];
    int strength_b = preferences[pair_b->winner][pair_b->loser];

    // Sort in descending order of strength
    return strength_b - strength_a;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    qsort(pairs, pair_count, sizeof(pair), compare_pairs);
}

// Checks if adding an edge from winner to loser would create a cycle in the graph
bool cycle_exist(int winner, int loser)
{
    if (winner == loser)
        return true;

    for (int i = 0; i < candidate_count; i++)
        if (locked[loser][i])
            if (cycle_exist(winner, i))
                return true;

    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
        if (!cycle_exist(pairs[i].winner, pairs[i].loser))
            locked[pairs[i].winner][pairs[i].loser] = true;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                is_source = false;
                break;
            }
        }
        if (is_source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
