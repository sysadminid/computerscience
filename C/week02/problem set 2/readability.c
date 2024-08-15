#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text
    float letters = count_letters(text);
    float words = count_words(text);
    float sentences = count_sentences(text);

    // Compute the Coleman-Liau index
    int index =
        round(0.0588 * (letters / words * 100 * 1.0) - 0.296 * (sentences / words * 100) - 15.8);

    // Print the grade level
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 1 && index <= 15)
    {
        printf("Grade %i\n", index);
    }
    else
    {
        printf("Grade 16+\n");
    }
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0, t = strlen(text); i < t; i++)
    {
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z'))
        {
            letters++;
        }
    }
    // Return the number of letters in text
    return letters;
}

int count_words(string text)
{
    int words = 1;
    for (int i = 0, t = strlen(text); i < t; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
    }
    // Return the number of words in text
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0, t = strlen(text); i < t; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    // Return the number of sentences in text
    return sentences;
}
