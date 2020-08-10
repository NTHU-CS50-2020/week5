#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
const unsigned int N = 100000;

// Hash table
node *table[N];
unsigned long long count = 0;
unsigned int max_hash = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char w[LENGTH+1];
    strcpy(w, word);
    for(int i = 0; w[i]; i++){
        w[i] = tolower(w[i]);
    }
    unsigned int key = hash(w);
    if(table[key]==NULL) return false;
    node *tmp = table[key];

    while(tmp->next != NULL)
    {
        if(!(strcmp(tmp->word, w))) return true;
        tmp = tmp->next;
    }
    if(!(strcmp(tmp->word, w))) return true;
    
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
	// sdbm
    unsigned long hash = 0;
    int c;
	
    while ((c = *word++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash%N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (!file) return false;

    char buffer[LENGTH + 2];

    while (fgets(buffer, LENGTH + 2, file)) {
        buffer[strlen(buffer) - 1] = '\0';
        unsigned int key = hash(buffer);
        node *tmp = malloc(sizeof(node));
        strcpy(tmp->word, buffer);
        tmp->next = table[key];
        table[key] = tmp;
        count++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < N; i++){
        node *tmp = table[i];
        if(!tmp) continue;
        do {
            node *cur = tmp->next;
            free(tmp);
            tmp = cur;
        }while(tmp);
    }
    return true;
}