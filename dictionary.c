// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

int px = 0;
int counter = 0;
bool loaded = false;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }

        strcpy(new_node->word, word);
        new_node->next = NULL;

        int x = hash(word);
        px = x;

        if (hashtable[x] == NULL)
        {
            hashtable[x] = new_node;
        }
        else
        {
            for (node *y = hashtable[x]; y != NULL; y = y->next)
            {
                if (y->next == NULL)
                {
                    y->next = new_node;
                    break;
                }
            }
        }

        counter++;
    }

    loaded = true;

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded == true)
    {
        return counter;
    }

    return 0;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int x = hash(word);

    for (node *y = hashtable[x]; y != NULL; y = y->next)
    {
        if (strcasecmp(y->word, word) == 0)
        {
            return true;
        }

        y = y->next;

        return false;
    }

    return true;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *y = hashtable[i];

        while (y != NULL)
        {
            node *temp = y;
            y = y->next;
            free(temp);
        }
    }

    return true;
}
