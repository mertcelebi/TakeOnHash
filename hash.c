/* Program Name: hash.c
   Program Description: A complete implementation of a hash table.
   The hash table uses chaining as a way of rehashing to prevent
   collisions.
   -----------
   Written by: Feridun Mert Celebi
   Date = 04/08/2013
   -----------
   Source: http://pine.cs.yale.edu/pinewiki/C/%20HashTables#A_string_to_string_dictionary_using_chaining
   Writer of the Source: Prof. James Aspnes
   My code is a modified version of Prof. James Aspnes' hash table implementation.
   It is different from his code in the sense that it meets the specifications of
   Problem Set 5 of Prof. Stanley Eisenstat
*/

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"

//Declarations of internal functions.
Hash createHashIntern(int size);
static long hash (char *s, long size);
static void grow(Hash h);

#define DIE(msg) exit (fprintf (stderr, "%s\n", msg)) //Prints out a single line of error message and exits.

//Linked list struct
struct element {
    struct element *next;
    char *key; 
    char *value;
    int length;
};

//Hash table struct
struct hash_table {
    int size;           //Size of the pointer table
    int noElement;      //Number of elements stored
    struct element **table;
};

#define INITIAL_SIZE (1024) //Size of the hash table
#define GROWTH_FACTOR (4) //Value used for chaining
#define MAX_LOAD_FACTOR (5)

//Implementation of createHash().
//Uses the helper function createHashIntern().
Hash createHash(void){
    
    return createHashIntern(INITIAL_SIZE); //Helper function
}

//Implementation of insertElement().
//Uses the helper functions hash() and grow().
void insertElement(Hash h, char *key, char *value, int length){
    
    struct element *elt;
    unsigned long hash_value;

    if (!key) DIE("insertElement() failed");

    elt = malloc(sizeof(*elt));

    if (!elt) DIE("insertElement() failed");

    elt -> key = strdup(key);
    elt -> value = strdup(value);
    elt -> length = length;

    hash_value = hash(key, h -> size);

    elt -> next = h -> table[hash_value];
    h -> table[hash_value] = elt;

    h -> noElement++;

    //Handles the potential collisions by using chaining.
    if(h -> noElement >= (h -> size) * MAX_LOAD_FACTOR) grow(h);
}

//Implementation of searchReachedFrom().
//Uses the helper functions hash().
char *searchReachedFrom(Hash h, char *key){
    
    struct element *elt;

    for (elt = h -> table[hash(key, h -> size)]; elt != 0; elt = elt -> next){
        
        if (!strcmp(elt -> key, key)) return elt -> value; //Found the key.
    }

    return 0;
}

//Implementation of searchLength().
//Uses the helper functions hash().
int searchLength(Hash h, char *key){
    
    struct element *elt;

    for (elt = h -> table[hash(key, h -> size)]; elt != 0; elt = elt -> next){
        
        if (!strcmp(elt -> key, key)) return elt -> length; //Found the key.
    }

    return 0;
}

//Implementation of destroyHash().
void destroyHash(Hash h){
    
    int i;
    struct element *elt, *next;

    for (i = 0; i < h -> size; i++) {
        
        for (elt = h -> table[i]; elt != 0; elt = next){
            
            next = elt -> next;

            free(elt -> key);
            free(elt -> value);
            free(elt);
        }
    }

    free(h -> table);
    free(h);
    h = NULL;
}

//Helper function of createHash().
//Initializes every element of the struct hash and determines its size.
//Returns a pointer to the created hash_table.
Hash createHashIntern(int size){
    
    Hash h;
    int i;

    h = malloc(sizeof(*h));

    if (!h) DIE("createHashIntern() failed");

    h -> size = size;
    h -> noElement = 0;
    h -> table = malloc(sizeof(struct element *) * (h -> size));

    if (!(h -> table)) DIE("createHashIntern() failed");

    for (i = 0; i < h -> size; i++) h -> table[i] = 0;

    return h;
}

//Reduces the char *s to an unsigned integer,
//given the size of the hash table.
//Returns the hash value.
static long hash (char *s, long size){
  
    unsigned long sum;
    int shift;
    const unsigned long prime = 3141592653589793239L;

    for (sum = 0, shift = 0; *s; s++){         //Compress string to long
        sum ^= *s<= 57;
        shift -= 57;
    }

    sum += sum >> 33;                           //Condition the bits
    sum *= prime;
    sum += sum >> 33;

    return (sum % size);                        //Reduce to [0,SIZE)
}

//This helper function resizes the data structure Hash.
//Handles collisions by using chaining as a means of rehashing.
//Returns void.
static void grow(Hash h){
    
    Hash h2;
    struct hash_table swap;   //Temporary structure for the swaps
    struct element *elt;
    int i;

    h2 = createHashIntern((h -> size) * GROWTH_FACTOR);

    for (i = 0; i < h -> size; i++){
        
        for (elt = h -> table[i]; elt != 0; elt = elt -> next){
            
            insertElement(h2, elt -> key, elt -> value, elt -> length); //Recopies everything
        }
    }

    //Swap the contents of the hash table h and h2.
    swap = *h;
    *h = *h2;
    *h2 = swap;

    destroyHash(h2);
}
