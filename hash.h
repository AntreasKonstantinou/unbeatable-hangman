/**
 * @file hash.h
 * @brief Header file for hash table operations module
 * 
 * Contains definitions and function declarations for hash table
 * used to distribute words into word families based on letter patterns.
 * 
 */

#ifndef HASH_H
#define HASH_H

#include "list.h"

/**
 * @brief Hash table structure for word families
 * 
 * Represents a hash table where each slot contains a linked list
 * of words that share the same letter pattern for a given guess.
 */
typedef struct {
    LIST **slot;      /**< Array of pointers to linked lists (buckets) */
    int size;         /**< Number of slots in hash table (2^wordLength) */
    int wordLength;   /**< Length of each word in the table */
} HASHTABLE;

/** 
 * @brief Create a hash table for words of given length
 * 
 * Allocates and initializes a hash table with 2^wordLength slots
 * to accommodate all possible letter position patterns.
 * 
 * @param wordLength Length of each word
 * @return HASHTABLE* Pointer to created hash table, NULL on failure
 */
HASHTABLE *createHash(int wordLength);

/** 
 * @brief Calculate base^exponent for hash table size calculation
 * 
 * Utility function for integer exponentiation used to calculate
 * hash table size and hash indices.
 * 
 * @param base Base number
 * @param exponent Exponent to raise base to
 * @return int Result of base^exponent
 */
int findPow(int base, int exponent);

/** 
 * @brief Calculate hash index for a word based on letter positions
 * 
 * Generates a unique index for each possible letter position pattern
 * using binary representation (1 for letter present, 0 for absent).
 * 
 * @param word Word to hash
 * @param letter Letter to check positions for
 * @param wordLength Length of the word
 * @return int Index in range [0, 2^wordLength - 1]
 */
int findIndex(const char *word, char letter, int wordLength);

/** 
 * @brief Insert a word into the hash table at specified index
 * 
 * Adds a word to the appropriate bucket in the hash table
 * based on its calculated index.
 * 
 * @param hash Hash table to insert into
 * @param word Word to insert
 * @param index Hash index (bucket number)
 */
void insertHash(HASHTABLE *hash, const char *word, int index);

/** 
 * @brief Distribute all words from a list into hash table based on letter positions
 * 
 * Processes all words in a list and distributes them into the hash table
 * buckets based on their letter position patterns.
 * 
 * @param list Source list of words
 * @param letter Letter to use for pattern matching
 * @param hash Destination hash table
 */
void ListToHash(LIST *list, char letter, HASHTABLE *hash);

/** 
 * @brief Find and return the largest bucket (list) in the hash table
 * 
 * Implements the strategic selection of the largest word family
 * to maintain maximum flexibility for the computer opponent.
 * 
 * @param hash Hash table to search
 * @return LIST* Pointer to the largest list (bucket), NULL if empty
 */
LIST *giveLargestBucket(HASHTABLE *hash);

/** 
 * @brief Free all memory used by hash table including all lists
 * 
 * Completely deallocates the hash table structure and all
 * associated linked lists and their contents.
 * 
 * @param hash Hash table to free
 */
void freeHash(HASHTABLE *hash);

/** 
 * @brief Print statistics about the hash table (for debugging)
 * 
 * Displays information about hash table usage including
 * number of non-empty slots and total words.
 * 
 * @param hash Hash table to analyze
 */
void printHashStats(HASHTABLE *hash);

#endif /* HASH_H */