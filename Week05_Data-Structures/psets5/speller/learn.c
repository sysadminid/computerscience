#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const unsigned int LENGTH = 45;
const unsigned int N = 143091;

unsigned int hash(const char *word);
bool load(const char *dictionary);

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

node *table[N];

int main(void)
{
    // TODO
    bool result = load("dictionaries/large");

    printf("%b", result);
}

unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int hash = 0;
    for (int i = 0; word[i] != '\0'; i++) {
        hash = hash * 3 + word[i];
    }
    return N - (hash % N) - 1;
}

bool load(const char *dictionary)
{
    // TODO
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        printf("No enough memory to load dictionary.");
        return false;
    }

    char buffer[LENGTH + 1];
    while (fscanf(dict, "%s", buffer) != EOF)
    {
        node *item = malloc(sizeof(node));
        if (item == NULL)
        {
            printf("No enough memory to load dictionary.");
            return false;
        }

        int hash_value = hash(buffer);

        if (table[hash_value]->next != NULL)
        {
            strcpy(item->word, buffer);
            item->next = table[hash_value]->next;
            table[hash_value]->next = item;
        }
        else
        {
            strcpy(item->word, buffer);
            item->next = NULL;
        }

        free(item);
    }

    fclose(dict);
    return true;
}
