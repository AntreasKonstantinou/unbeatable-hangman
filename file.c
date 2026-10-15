/**
 * @file file.c
 * @brief File operations for reading word lists and word length detection
 * 
 * This module handles reading word files where the first line contains
 * the word length and subsequent lines contain words of that length.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/**
 * @brief Copies words from file into linked list
 */
void copyFile(FILE *file, LIST *list) {
    if (!file || !list) return;
    
    // Get the word length from the first line
    int wordLength;
    if (fscanf(file, "%d", &wordLength) != 1) {
        return; // Couldn't read the word length
    }
    
    // Allocate buffer for reading words (+1 for null terminator)
    char *word = malloc(wordLength + 1);
    if (!word) return;
    
    // Read each word until EOF
    while (fscanf(file, "%s", word) == 1) {
        insertList(list, word);
    }
    
    free(word); // Free the temporary buffer
}

/**
 * @brief Gets word length from file
 */
int wordSize(FILE *file) {
    if (!file) return -1;
    
    int wordLength;
    
    // Read the first integer from the file (word length)
    if (fscanf(file, "%d", &wordLength) == 1) {
        return wordLength;
    }
    
    return -1; // Error reading the word length
}

#ifdef TEST_FILE
/**
 * @brief Test driver for file operations
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
    
    // Reset file pointer to beginning and read length again
    rewind(file);
    
    // Create a list and copy the file contents
    LIST *list = createList();
    copyFile(file, list);
    
    // Print the actual list size
    int actualSize = listSize(list);
    printf("Words copied to list: %d\n", actualSize);
    
    // Print first word to verify
    printf("First word in list: %s\n", listFirst(list));
    
    // Close the file
    fclose(file);
    
    // Free the list
    freeList(list);
    
    return 0;
}
#endif