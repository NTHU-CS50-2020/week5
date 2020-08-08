// Implements a dictionary's functionality

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
const unsigned int N = 26;

// Hash table
node *table[N];
unsigned long long word_count = 0;
unsigned int max_hash = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char str[LENGTH+1];
    strcpy(str, word);
    for(int i = 0; str[i]; i++){
        str[i] = tolower(str[i]);
    }
    //printf("%s->%s\n", word, str);
    unsigned int key = hash(str);
    if(key>26) return false;
    node *tmp = table[key];
    while(tmp->next != NULL)
    {
        if(strcmp(tmp->word, str)==0)
        {
            return true;
        }
        tmp = tmp->next;

    }

    if(strcmp(tmp->word, str)==0)
    {
        return true;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO : update hash func to get faster
    //https://stackoverflow.com/questions/7700400/whats-a-good-hash-function-for-english-words
    unsigned int hash = *word - 'a';
    max_hash = (max_hash>=hash)?max_hash:hash;
    //printf("%s with hash %u\n", word, hash);
    return hash;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //printf("%s\n", dictionary);
    FILE *file = fopen(dictionary, "r");

    if ( file == NULL )
    {
        return false;
    }

    char buffer[LENGTH + 2];
    bool success = true;
    node *EachKeyEnd[26];

    while (fgets(buffer, LENGTH + 2, file)) {
        //printf("%s\n", buffer);
        buffer[strlen(buffer) - 1] = '\0';
        unsigned int key = hash(buffer);
        if( !table[key] )
        {
            //printf("%p\n", table[key]);
            table[key] = malloc(sizeof(node));
            strcpy(table[key]->word, buffer);
            table[key]->next = NULL;
            EachKeyEnd[key] = table[key];
        }
        else
        {
            node *tmp = EachKeyEnd[key];
            //while(tmp->next!=NULL) tmp = tmp->next;//too slow
            //tmp = malloc(sizeof(node));
            //tmp->next->word = buffer;
            tmp->next = malloc(sizeof(node));
            strcpy(tmp->next->word, buffer);
            tmp->next->next = NULL;
            EachKeyEnd[key] = tmp->next;
            //printf("%s\n", EachKeyEnd[key]->word);
        }
        word_count ++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    //printf("#words:%llu\nmax hash number:%u\n",word_count, max_hash);
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i<N; i++)
    {
        node *list = table[i];
        if( !list ) continue;
        while(list!=NULL)
        {
            node *tmp = list->next;
            free(list);
            list = tmp;
        }
    }
    return true;
}
