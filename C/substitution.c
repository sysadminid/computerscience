#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

bool isvalidLen(string key);
bool isnotAlpha(string key);
bool ischarRepeat(string key);
string encipher(string key, string plaintext);

int main(int argc, string argv[])
{
    string key = argv[1];

    // // Validate the key
    if (argv[1] == NULL || argc > 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    else if (!(isvalidLen(key)))
    {
        printf("Key must be contain 26 characters.\n");
        return 1;
    }
    else if (isnotAlpha(key))
    {
        printf("Key must only contain alphabetic characters.\n");
        return 1;
    }
    else if (ischarRepeat(key))
    {
        printf("Key must not contain repeated characters.\n");
        return 1;
    }

    // Get plaintext
    string text = get_string("plaintext: ");

    // Encipher
    int len = strlen(text);
    char ciphertext[len];

    for (int i = 0; i < len; i++)
    {
        if (isupper(text[i]))
        {
            int z = text[i] - 'A';
            if (isupper(key[z]))
            {
                ciphertext[i] = key[z];
            }
            else
            {
                z = key[z] - ('a' - 'A');
                ciphertext[i] = (char) z;
            }
        }
        else if (islower(text[i]))
        {
            int z = text[i] - 'a';
            if (islower(key[z]))
            {
                ciphertext[i] = key[z];
            }
            else
            {
                z = key[z] + ('a' - 'A');
                ciphertext[i] = (char) z;
            }
        }
        else
        {
            ciphertext[i] = text[i];
        }
    }
    ciphertext[len] = '\0';

    // Print ciphertext
    printf("ciphertext: %s\n", ciphertext);
}

bool isvalidLen(string key)
{
    if (strlen(key) == 26)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isnotAlpha(string key)
{
    for (int i = 0, l = strlen(key); i < l; i++)
    {
        if (!(isalpha(key[i])))
        {
            return true;
        }
    }
    return false;
}

bool ischarRepeat(string key)
{
    for (int i = 0, l = strlen(key); i < l; i++)
    {
    int counter = 0;
    for (int j = 0; j < l; j++)
    {
        if (key[i] == key[j])
        {
            counter++;
        }
    }
    
    if (counter < 1 || counter > 1)
    {
        return true;
    }
    
    }
    return false;
}
