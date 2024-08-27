// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 143091;
unsigned int N_WORDS = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    char WORD[LENGTH + 1];
    unsigned int INDEX = hash(word);
    unsigned int WORD_LEN = strlen(word);

    for (int i = 0; i < WORD_LEN; i++)
    {
        WORD[i] = tolower(word[i]);
    }
    WORD[WORD_LEN] = '\0';

    node *CURSOR = table[INDEX];

    while (CURSOR != NULL)
    {
        if (strcasecmp(CURSOR->word, WORD) == 0)
        {
            return true;
        }

        CURSOR = CURSOR->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int HASH = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        HASH = HASH * 3 + tolower(word[i]);
    }
    return N - (HASH % N) - 1;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *DICT = fopen(dictionary, "r");
    if (DICT == NULL)
    {
        printf("No enough memory to load dictionary.");
        return false;
    }

    char BUFFER[LENGTH + 1];

    while (fscanf(DICT, "%s", BUFFER) != EOF)
    {
        node *ITEM = malloc(sizeof(node));
        if (ITEM == NULL)
        {
            printf("No enough memory.");
            unload();
            return false;
        }

        int HASH_INDEX = hash(BUFFER);
        strcpy(ITEM->word, BUFFER);

        if (table[HASH_INDEX] == NULL)
        {
            ITEM->next = NULL;
            table[HASH_INDEX] = ITEM;
        }
        else
        {
            ITEM->next = table[HASH_INDEX]->next;
            table[HASH_INDEX]->next = ITEM;
        }

        N_WORDS++;
    }

    fclose(DICT);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return N_WORDS;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *CURRENT = NULL, *TEMP = NULL;

    for (int i = 0; i < N; i++)
    {
        CURRENT = table[i];
        while (CURRENT != NULL)
        {
            TEMP = CURRENT;
            CURRENT = TEMP->next;
            free(TEMP);
        }
        free(CURRENT);
    }
    return true;
}
