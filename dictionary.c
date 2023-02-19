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

// Function used
void freedome(node *current);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Get the hash number
    int h = hash(word);
    node *current = table[h];

    // Go through the chain until word found or end of chain
    while (current->next != NULL)
    {
        current = current->next;
        if (strcasecmp(word, current->word) == 0)
        {
            return true;
        }
    }

    // Return false if chain ended and word not found
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // I am using the same chain method shown in the shorts.
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open the dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Initlize the hash table
    int a = initlize_node_table();
    if (a == 1)
    {
        return false;
    }

    // Buffer to read the words
    char buffer[LENGTH + 1];
    // char b;

    // Get file pointer value to check if it reached EOF
    int c = getc(dict);

    // Make sure point is at the start of the file
    fseek(dict, 0, SEEK_SET);

    // Read until end of file
    while (c != EOF)
    {
        // Read the word in that line
        for (int i = 0; i < LENGTH + 1; i++)
        {
            fread(&buffer[i], sizeof(char), 1, dict);
            if (buffer[i] == 10)
            {
                buffer[i] = '\0';
                break;
            }
            else
            {
                buffer[i] = toupper(buffer[i]);
            }
        }

        // Create a new node to store the value
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            return false;
        }
        strcpy(new->word, buffer);
        new->next = NULL;

        // Find the hash value for the word just read.
        int h = hash(buffer);

        // Go to the hash table location to store the word
        node *current = table[h];

        if (current->next == NULL)
        {
            //  connect the new node if the hash table location is touched before
            current->next = new;
        }
        else
        {
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = new;
        }
        c = getc(dict);
        fseek(dict, -1, SEEK_CUR);
    }

    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    int sz = 0;
    node *current;
    for (int i = 0; i < 26; i++)
    {
        current = table[i];
        while (current->next != NULL)
        {
            sz++;
            current = current->next;
        }
    }
    return sz;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < 26; i++)
    {
        freedome(table[i]);
    }

    return true;
}

int initlize_node_table(void)
{
    for (int i = 0; i < 26; i++)
    {
        // Allocate memory
        table[i] = malloc(sizeof(node));
        if (table[i] == NULL)
        {
            return 1;
        }
        table[i]->word[0] = (65 + i);
        table[i]->next = NULL;
    }
    return 0;
}

void freedome(node *current)
{
    // Recall untill reaching the end of the chain
    if (current->next != NULL)
    {
        freedome(current->next);
        current->next = NULL;
    }

    // freeing the current node
    free(current);
}
