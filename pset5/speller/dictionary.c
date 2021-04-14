// Implements a dictionary's functionality

#include <stdbool.h>
#include <ctype.h>
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
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    node *temp = table[hash(word)];
    bool found = false;

    //  If node words are present
    if (temp != NULL)
    {
        // Compare word and move to next node if it doesn't match
        do
        {
            // If it does match, return true
            if (strcasecmp(temp->word, word) == 0)
            {
                return true;
            }
            temp = temp->next;
        }
        while (temp != NULL);
    }
    // If no word found, return false
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Retrun index of letter in the alphabet
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open file which is named 'dictionary'
    FILE *inptr = fopen(dictionary, "r");
    if (!inptr)
    {
        fprintf(stderr, "Could not open %s.\n", dictionary);
        return false;
    }

    char current_word[LENGTH];
    char current_char;
    int index = 0;
    int hash_index = 0;
    node *temp;

    do
    {
        current_char = fgetc(inptr);
        if (current_char == EOF)
        {
            break;
        }
        else if (current_char == '\n')
        {
            current_word[index] = '\0';
            node *new_node = malloc(sizeof(node));
            if (new_node == NULL)
            {
                return false;
            }
            strcpy(new_node->word, current_word);
            new_node->next = NULL;
            hash_index = hash(current_word);
            if (table[hash_index] == NULL)
            {
                table[hash_index] = new_node;
            }
            else
            {
                temp = table[hash_index];
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = new_node;
            }
            index = 0;
        }
        else if (isalpha(current_char) || current_char == '\'')
        {
            current_word[index] = current_char;
            index++;
        }
    }
    while (1);

    fclose(inptr);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    node *temp;
    int size = 0;

    // Traverse each element of array
    for (int i = 0; i < N; i++)
    {
        //  If no nodes for this element, skip
        temp = table[i];
        if (temp != NULL)
        {
            // Increment size and move to next node if there is one
            do
            {
                size++;
                temp = temp->next;
            }
            while (temp != NULL);
        }
    }
    // Return final size
    return size;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *temp;
    node *prev;

    // Traverse each element of array
    for (int i = 0; i < N; i++)
    {
        //  If no nodes for this element, skip
        if (table[i] != NULL)
        {
            //
            temp = table[i];
            while (temp->next != NULL)
            {
                prev = temp;
                temp = temp->next;
                free(prev);
            }
            free(temp);
            table[i] = NULL;
        }
    }

    return true;
}
