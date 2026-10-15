/**
 * @file game.c
 * @brief Hangman game implementation with strategic computer opponent
 * 
 * This module implements the "unbeatable hangman" game where the computer
 * uses word families to always maintain multiple possible secret words,
 * making it difficult for the player to win.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "hash.h"
#include "game.h"

/**
 * @brief Check if a letter was already guessed
 */
int wasLetterGuessed(char letter, int* guessedLetters) {
    if (!guessedLetters) return 0;
    return guessedLetters[letter - 'a'] != 0;
}

/**
 * @brief Update the pattern based on letter positions in largest bucket
 */
void updatePattern(char* pattern, char letter, LIST *largestBucket, int wordLength) {
    if (!largestBucket || !largestBucket->head) return;
    
    // Use the first word in the largest bucket to determine letter positions
    const char *sampleWord = largestBucket->head->name;
    for (int i = 0; i < wordLength; i++) {
        if (sampleWord[i] == letter) {
            pattern[i] = letter;
        }
    }
}

/**
 * @brief Check if the game is won (no underscores in pattern)
 */
int isGameWon(const char* pattern, int wordLength) {
    for (int i = 0; i < wordLength; i++) {
        if (pattern[i] == '_') {
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Get a valid letter guess from the player
 */
char getPlayerGuess(void) {
    char input[100];
    char guess;
    
    while (1) {
        printf("Guess a letter: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            // End of input (e.g. Ctrl+D): stop instead of looping forever
            printf("\nNo more input. Exiting the game.\n");
            exit(0);
        }
        
        // Check if input is exactly one character (plus newline)
        if (strlen(input) == 2 && input[1] == '\n') {
            guess = input[0];
            
            // Validate it's a lowercase letter
            if (guess >= 'a' && guess <= 'z') {
                return guess;
            }
        }
        
        printf("Wrong input!\n");
    }
}

/**
 * @brief Count how many positions a letter appears in a word
 */
int countLetterPositions(const char* word, char letter, int wordLength) {
    int count = 0;
    for (int i = 0; i < wordLength; i++) {
        if (word[i] == letter) {
            count++;
        }
    }
    return count;
}

/**
 * @brief Check if the largest bucket contains the guessed letter
 */
int doesBucketContainLetter(LIST *bucket, char letter) {
    if (!bucket || !bucket->head) return 0;
    
    // All words in the same bucket should have the same letter pattern
    // So we can just check the first word
    return strchr(bucket->head->name, letter) != NULL;
}

/**
 * @brief Main game loop function
 */
void play(LIST *wordList, int wordLength, int maxGuesses) {
    if (!wordList || wordLength <= 0 || maxGuesses <= 0) {
        printf("Error: Invalid parameters for play()\n");
        return;
    }
    
    int remainingGuesses = maxGuesses;
    int* guessedLetters = calloc(26, sizeof(int)); // Track a-z guesses
    
    // Initialize pattern with all underscores
    char* pattern = malloc(wordLength + 1);
    for (int i = 0; i < wordLength; i++) {
        pattern[i] = '_';
    }
    pattern[wordLength] = '\0';
    
    // Current working list of words (starts with all words)
    LIST *currentWordList = createList();
    if (!currentWordList) {
        printf("Error: Could not create word list\n");
        free(guessedLetters);
        free(pattern);
        return;
    }
    
    // Copy all words to current list
    NODE *currentNode = wordList->head;
    while (currentNode) {
        insertList(currentWordList, currentNode->name);
        currentNode = currentNode->next;
    }

    int firstGuess = 1; // Flag to handle first guess display differently
    
    // Main game loop - continues until guesses run out or player wins
    while (remainingGuesses > 0 && !isGameWon(pattern, wordLength)) {
        printf("-------------------------------\n");
        
        // Display word count (skip for first guess)
        if (firstGuess == 1)
            firstGuess = 0;
        else
            printf("Words possible: %d\n", currentWordList->size);
            
        // Display current game state
        printf("Progress: ");
        for (int i = 0; i < wordLength; i++) {
            printf("%c", pattern[i]);
        }
        printf("\n");
        printf("Number of Guesses: %d\n", remainingGuesses);
        
        // Get player's guess
        char guess = getPlayerGuess();
        
        // Check if letter was already guessed
        if (wasLetterGuessed(guess, guessedLetters)) {
            printf("Letter %c was given before\n\n", guess);
            continue;
        }
        
        // Add to guessed letters array
        guessedLetters[guess - 'a'] = 1;
        
        // Create hash table for current guess to distribute words into families
        HASHTABLE *hash = createHash(wordLength);
        if (!hash) {
            printf("Error: Could not create hash table\n");
            break;
        }
        
        // Distribute words into hash buckets based on the guessed letter positions
        ListToHash(currentWordList, guess, hash);
        
        // Find the largest bucket (computer's strategic choice)
        LIST *largestBucket = giveLargestBucket(hash);
        if (!largestBucket || largestBucket->size == 0) {
            printf("Error: No valid buckets found\n");
            freeHash(hash);
            break;
        }
        
        // Check if the largest bucket contains the guessed letter
        int letterInBucket = doesBucketContainLetter(largestBucket, guess);
        
        // Update game state based on computer's choice
        if (letterInBucket) {
            printf("You guessed a letter correctly\n\n");
            
            // Update pattern to reveal ONLY the specific letter positions
            updatePattern(pattern, guess, largestBucket, wordLength);
        } else {
            printf("%c not in secret word\n\n", guess);
            remainingGuesses--;
        }
        
        // Replace current word list with the largest bucket (strategic reduction)
        freeList(currentWordList);
        currentWordList = createList();
        
        // Copy words from largest bucket to new current list
        NODE *bucketNode = largestBucket->head;
        while (bucketNode) {
            insertList(currentWordList, bucketNode->name);
            bucketNode = bucketNode->next;
        }

        // First, set the largest bucket pointer to NULL in the hash table so we don't free it
        LIST *newCurrentList = createList();
        if (newCurrentList && largestBucket) {
            NODE *currentNode = largestBucket->head;
            while (currentNode) {
                insertList(newCurrentList, currentNode->name);
                currentNode = currentNode->next;
            }
        }

       freeHash(hash);

       // Replace the current word list with our new clean list
        freeList(currentWordList);
        currentWordList = newCurrentList;
    }
    
    // Game over - determine win/lose state and display final result
    if (isGameWon(pattern, wordLength)) {
        printf("**************************************************\n");
        printf("Congratulations, you win the game\n");
        printf("The secret word is: %s\n", currentWordList->head->name);
        printf("**************************************************\n");
    } else {
        printf("**************************************************\n");
        printf("You run out of guesses. You lost\n");
        if (currentWordList && currentWordList->head) {
            printf("The secret word was: %s\n", currentWordList->head->name);
        } else {
            printf("The secret word was: [unknown]\n");
        }
        printf("**************************************************\n");
    }
    
    // Cleanup all dynamically allocated memory
    free(guessedLetters);
    free(pattern);
    freeList(currentWordList);
}