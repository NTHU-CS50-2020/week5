// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
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
//const unsigned int N = 1;
unsigned int words=0;
#define SIZE 1000


// Hash table
node *table[SIZE];

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    for (int i=0; word[i]!= '\0'; i++)
    {
        hash = 31*hash + word[i];//>?<
    }
    return hash % SIZE;//output 0,N-1 >?<
}

struct node *getNewNode(const char* word)
{
    struct node *newNode=(struct node*)malloc(sizeof(struct node));
     if(newNode==NULL)
    {
        return false;
    }
    newNode->next=NULL;
    strcpy(newNode->word, word);
    
    return newNode;
}

int strcasecmp(const char *s1, const char *s2)
{
int c1, c2;
do {
c1 = tolower(*s1++);
c2 = tolower(*s2++);
} while(c1 == c2 && c1 != 0);
return c1 - c2;
}


// Returns true if word is in dictionary else false
bool check(const char *word)///////////////////////////////////
{
//declares a string to hold a word converted to lowercase for hashing
    char tword[LENGTH+1];
    
    int n=strlen(word);
    
    for(int i=0; i<n+1; i++)
    {
        tword[i]=tolower(word[i]);
    }
    
    //creates a node pointer pointing to the first node in hashtable hash_func-th element
    node *cursor=table[hash(tword)];
    
    //search for a word in a linked list linked to hashtable element
    while(cursor!=NULL)
    {
        if(strcasecmp(cursor->word, word)==0)
        {
            return true;
        }
        else
        {
            cursor=cursor->next;
        }
    }

    return false;
    


}



// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    
    FILE *file = fopen(dictionary, "r");
    if ( file == NULL )
    {
        return false;
    }
    char word[47];
    
    unsigned int hashi=0;
    
    do
    {
        hashi=hash(word);
        
        /*node *store=malloc(sizeof(node));
        if ( store == NULL )
        {
            return false;
        }*/
        
        if(table[hashi]==NULL)
        {
            table[hashi]=getNewNode(word);
        
        }
        else
        {
            node *newNode=getNewNode(word);
            newNode->next=table[hashi];
            table[hashi]=newNode;
        
        }
        words++;
        
    }while(fscanf(file,"%s",word)!=EOF);

    
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
     for(int i=0; i<SIZE; i++)
    {
        node *cursor=table[i];
        node *tmp=cursor;
        do{
            free(cursor);
            tmp=cursor;
            cursor=cursor->next;
            free(tmp);
        }while(cursor!=NULL);
    }
    // TODO
    return false;
}
