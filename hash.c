/**
 * @file hash.c
 * @brief Hash table implementation for word family distribution
 * 
 * This module implements hash table operations used to distribute
 * words into families based on letter position patterns, which is
 * key to the computer's strategic gameplay.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "hash.h"
#include "file.h"

/**
 * @brief Create a hash table for words of given length
 */
HASHTABLE *createHash(int wordLength) {
    
    HASHTABLE *ht = malloc(sizeof(HASHTABLE));
    if (!ht) return NULL;
    
    ht->wordLength = wordLength;
    ht->size = findPow(2, wordLength); // 2^wordLength possible patterns
    ht->slot = calloc(ht->size, sizeof(LIST *));
    
    if (!ht->slot) { 
        free(ht); 
        return NULL; 
    }
    
    /* Create empty lists for every slot */
    for (int i = 0; i < ht->size; ++i) {
        ht->slot[i] = createList();
        if (!ht->slot[i]) {
            /* Cleanup on failure - free all previously allocated lists */
            for (int j = 0; j < i; ++j) {
                freeList(ht->slot[j]);
            }
            free(ht->slot);
            free(ht);
            return NULL;
        }
    }
    return ht;
}

/**
 * @brief Calculate base^exponent for integer values
 */
int findPow(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

/**
 * @brief Calculate hash index for a word based on letter positions
 */
int findIndex(const char *word, char letter, int wordLength) {
    int index = 0;
    for (int i = 0; i < wordLength; i++) {
        if (word[i] == letter) {
            index += findPow(2, i); // Add 2^i for position i
        }
    }
    return index;
}

/**
 * @brief Insert a word into the hash table at specified index
 */
void insertHash(HASHTABLE *hash, const char *word, int index) {
    if (!hash || !word || index < 0 || index >= hash->size) return;
    insertList(hash->slot[index], word);
}

/**
 * @brief Distribute all words from a list into hash table based on letter positions
 */
void ListToHash(LIST *list, char letter, HASHTABLE *hash) {
    if (!list || !hash) return;
    
    NODE *tmp = list->head;
    while (tmp) {
        int index = findIndex(tmp->name, letter, hash->wordLength);
        insertHash(hash, tmp->name, index);
        tmp = tmp->next;
    }
}

/**
 * @brief Find and return the largest bucket in the hash table
 */
LIST *giveLargestBucket(HASHTABLE *hash) {
    if (!hash || hash->size == 0) return NULL;
    
    int maxSize = 0;
    int maxIndex = 0;
    
    // Find the bucket with maximum number of words
    for (int i = 0; i < hash->size; i++) {
        if (hash->slot[i] && hash->slot[i]->size > maxSize) {
            maxSize = hash->slot[i]->size;
            maxIndex = i;
        }
    }
    
    return hash->slot[maxIndex];
}

/**
 * @brief Free all memory used by hash table
 */
void freeHash(HASHTABLE *hash) {
    if (!hash) return;
    
    if (hash->slot) {
        for (int i = 0; i < hash->size; i++) {
            if (hash->slot[i]) {
                freeList(hash->slot[i]);
            }
        }
        free(hash->slot);
    }
    free(hash);
}

/**
 * @brief Print statistics about the hash table
 */
void printHashStats(HASHTABLE *hash) {
    if (!hash) return;
    
    printf("Hash table stats:\n");
    printf("Word length: %d\n", hash->wordLength);
    printf("Total slots: %d\n", hash->size);
    
    int nonEmptySlots = 0;
    int totalWords = 0;
    
    // Count non-empty slots and total words
    for (int i = 0; i < hash->size; i++) {
        if (hash->slot[i] && hash->slot[i]->size > 0) {
            nonEmptySlots++;
            totalWords += hash->slot[i]->size;
        }
    }
    
    printf("Non-empty slots: %d\n", nonEmptySlots);
    printf("Total words: %d\n", totalWords);
}

#ifdef TEST_HASH
/**
 * @brief Test driver for hash table operations
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    // Open the file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }
    
    // Get the word length from the first line
    int wordLength = wordSize(file);
    printf("Word length from file: %d\n", wordLength);
    
    // Reset file pointer to beginning
    rewind(file);
    
    // Create a list and copy the file contents
    LIST *list = createList();
    copyFile(file, list);
    
    printf("Words in list: %d\n", listSize(list));
    
    // Test hash table creation
    HASHTABLE *hash = createHash(wordLength);
    if (!hash) {
        printf("Error: Could not create hash table\n");
        fclose(file);
        freeList(list);
        return 1;
    }
    
    printf("Hash table created with %d slots\n", hash->size);
    
    // Test distributing words by a specific letter
    char testLetter = 'e';
    printf("\nTesting distribution for letter '%c':\n", testLetter);
    ListToHash(list, testLetter, hash);
    
    // Print hash table statistics
    printHashStats(hash);
    
    // Find and display the largest bucket
    LIST *largest = giveLargestBucket(hash);
    if (largest) {
        printf("\nLargest bucket has %d words\n", largest->size);
        
        // Print first few words from largest bucket
        printf("First 3 words in largest bucket:\n");
        NODE *current = largest->head;
        int count = 0;
        while (current && count < 3) {
            printf("  %s\n", current->name);
            current = current->next;
            count++;
        }
        
        // Show the pattern for this bucket
        printf("Pattern for this bucket: ");
        for (int i = 0; i < wordLength; i++) {
            if (largest->head && strchr(largest->head->name, testLetter)) {
                // Check if this position has the letter in the first word
                if (largest->head->name[i] == testLetter) {
                    printf("%c", testLetter);
                } else {
                    printf("_");
                }
            } else {
                printf("_");
            }
        }
        printf("\n");
    }
    
    // Cleanup
    freeHash(hash);
    freeList(list);
    fclose(file);
    
    printf("\nHash test completed successfully!\n");
    return 0;
}
#endif