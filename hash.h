/* Program Name: hash.h
   Program Description: Define the header file for a hash table.
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

/* Initially the hash table should have M = 1024 entries. 
   Let N be the number of triples in the hash table. Whenever
   the loadaverage L = N/M exceeds 5 (for chaining), a new hash table 
   of four times the size; insert all triples in the old hash table 
   into the new hash table; and delete the old hash table.
*/

//In this case the header file declares the Hash object as a pointer to 
//struct hash_table. The names and types of the fields, which hold all 
//information about the hash table, are only declared in the file that
//implements the module.
typedef struct hash_table *Hash;

//Makes a call to an internal function, and creates a Hash,
//a pointer to the data structure hash_table.
//Returns a pointer to a hash_table.
Hash createHash();

//Inserts a new key-value pair into an existing hash table.
//Key is the POS and value is Reached-From
//Length is also a part of the value.
//Returns void.
void insertElement(Hash h, char *key, char *value, int length);

//Looks for the key in an existing hash table and returns the value
//(Reached-From in this case) associated with the key.
//Returns 0, if nothing is found.
char *searchReachedFrom(Hash h, char *key);

//Looks for the key in an existing hash table and returns the length
//(Length in this case) associated with the key.
//Returns 0, if nothing is found.
int searchLength(Hash h, char *key);

//Destroy the Hash h by freeing any storage that it uses (but not the the
//blocks of storage to which the string pointers point) and set the value
//of h to NULL.
void destroyHash(Hash h);
