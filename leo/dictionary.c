// Implements a dictionary's functionality
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 1000;
//const unsigned int N = (LENGTH +1)*'z';

// Hash table
node *table[N];

int total_word = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = table[index];
    while(cursor != NULL)
    {
        if(strcasecmp(cursor->word ,word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    return false;
}

// Hashes word to a number

unsigned int hash(const char *word)
{
    int first_word = 0, second_word = 0, sum = 0;
    first_word = tolower(word[0]) - 97;
    second_word = tolower(word[1]) - 97;
    sum = first_word * 26 + second_word;
    return (sum % N);
}

/*
unsigned int hash(const char *word)
{
    int sum = 0;
    for(int i = 0; i < strlen(word); i++)
    {
        sum += tolower(word[i]);
    }
    return(sum % N);
}
*/

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open dictionary file
    FILE *file_dictionary = fopen(dictionary, "r");
    if (file_dictionary == NULL)
    {
        printf("Could not open file\n");
        return false;
    }

    //read strings from file one at time

    char word[LENGTH + 1];
    while(fscanf(file_dictionary, "%s", word) != EOF)
    {
        //create a new node for each word
        node *n = malloc(sizeof(node));
        if(n == NULL)
        {
            return false;
        }
        strcpy(n->word, word);
        n->next = NULL;
        //hash word to obtain a hash value
        int index = hash(word);
        //insert node into hash table at that location
        if(table[index] == NULL)
        {
            table[index] = n;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
        }
        total_word += 1;
    }
    fclose(file_dictionary);
    return true;

}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return total_word;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        node *temp = table[i];

        while(cursor != NULL)
        {
            cursor = cursor->next;
            free(temp);
            temp = cursor;
        }
    }
    return true;
}