#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    // Check if there is one extra argument
    if (argc == 2)
    {
        // If so, check the number of characters in that argument
        if (strlen(argv[1]) != 26)
        {
            // If there is not 26 characters in that argument, return 1
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }
    else
    {
        // If there is not just one extra argument, return 1
        printf("Usage: %s key\n", argv[0]);
        return 1;
    }

    //int n = strlen(argv[1]);
    int key[26] = {};
    int capital_a = 'A';
    bool letters_used[26] = {};
    int current_letter = 0;
    // Loop through each character in the command line argument
    for (int i = 0; i < 26; i++)
    {
        current_letter = toupper(argv[1][i]);
        if (letters_used[current_letter - capital_a] == true)
        {
            printf("Duplicate letters used\n");
            return 1;
        }
        else if (current_letter < 'A' || current_letter > 'Z')
        {
            printf("Not a letter: %c\n", current_letter);
            return 1;
        }
        else
        {
            key[i] = current_letter - (capital_a + i);
            letters_used[current_letter - capital_a] = true;
        }
    }

    // Ask user for plain text to encode
    string plain_text = get_string("plaintext: ");
    printf("ciphertext: ");
    // Loop through each character of the plain text
    for (int j = 0, m = strlen(plain_text); j < m; j++)
    {
        if (plain_text[j] >= 'a' && plain_text[j] <= 'z')
        {
            // If the character is lowercase, find the encoded lowercase letter
            printf("%c", plain_text[j] + key[plain_text[j] - 'a']);
        }
        else if (plain_text[j] >= 'A' && plain_text[j] <= 'Z')
        {
            // If the character is uppercase, find the encoded uppercase letter
            printf("%c", plain_text[j] + key[plain_text[j] - 'A']);
        }
        else
        {
            // If not a letter, just print the same character out
            printf("%c", plain_text[j]);
        }
    }
    printf("\n");
    return 0;
}