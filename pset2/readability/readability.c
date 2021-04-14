#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>

int main(void)
{
    int characters = 0;
    char next_char;
    int words = 0;
    int sentences = 0;
    float grade_level = 0;
    float c_per_100_w = 0;
    float s_per_100_w = 0;

    // ask user for text and save it to 'exerpt'
    string exerpt = get_string("Text: ");
    for (int i = 0; i < strlen(exerpt); i++)
    {
        // go through each character and see if it is a letter
        if ((exerpt[i] >= 'a' && exerpt[i] <= 'z') || (exerpt[i] >= 'A' && exerpt[i] <= 'Z'))
        {
            // if so, increment 'characters'
            characters++;
            // look ahead to the next character
            next_char = exerpt[i + 1];
            if (next_char == ' ' || next_char == ',')
            {
                // if the next character is a space or a comma, then we just finished a word
                // increment 'words' and 'i' to ignore the space
                words++;
                i++;
            }
            else if (next_char == '.' || next_char == '!' || next_char == '?')
            {
                // if the next character is '.', '!', of '?', then we just finished a sentence
                // increment 'sentences', 'words', and 'i' to ignore the puncuation
                sentences++;
                words++;
                i++;
            }
        }
    }
    // index = 0.0588 * L - 0.296 * S - 15.8
    c_per_100_w = 100 * (float) characters / words;
    s_per_100_w = 100 * (float) sentences / words;
    grade_level = (0.0588 * c_per_100_w) - (0.296 * s_per_100_w) - 15.8;
    if (grade_level > 16)
    {
        printf("Grade 16+\n");
    }
    else if (grade_level < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(grade_level));
    }
    //printf("Characters: %i, Words: %i, Sentences: %i, Grade Level: %f\n", characters, words, sentences, grade_level);
    //printf("Characters/100words = %f, Sentences/100words = %f\n", c_per_100_w, s_per_100_w);
}