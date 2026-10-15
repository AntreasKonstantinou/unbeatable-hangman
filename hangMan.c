/**
 * @file hangMan.c
 * @brief Main program file for Unbeatable Hangman game
 * 
 * This is the main entry point for the Hangman game implementation
 * 
 * @author Andreas Konstantinou
 * @bug No known bugs
 *
 * Usage: ./hangMan fileName.txt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "file.h"
#include "hash.h"
#include "game.h"

/**
 * @brief Main function for Hangman game program
 * 
 * Handles command line arguments, file operations, user input,
 * and coordinates the overall game flow by calling appropriate functions.
 * 
 * @param argc Argument count - should be 2 for proper execution
 * @param argv Argument vector containing:
 *             - argv[0]: Program name
 *             - argv[1]: Word list filename
 * @return int Returns 0 on success, 1 on error
 */
int main(int argc, char *argv[]) {
    // Check command line arguments
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Print welcome message
    printf("**************************************************\n");
    printf("***             Welcome to Hangman             ***\n");
    printf("*** You are going to play against the computer ***\n");
    printf("**************************************************\n\n");

    // Open the word list file
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    // Get the word length from the first line of the file
    int wordLength = wordSize(file);
    if (wordLength <= 0) {
        printf("Error: Invalid word length in file\n");
        fclose(file);
        return 1;
    }

    // Reset file pointer to beginning to read the entire file
    rewind(file);

    // Create a linked list and load all words from the file
    LIST *wordList = createList();
    if (!wordList) {
        printf("Error: Could not create word list\n");
        fclose(file);
        return 1;
    }

    copyFile(file, wordList);
    fclose(file); // Close file after reading

    // Check if any words were successfully loaded
    int wordCount = listSize(wordList);
    if (wordCount == 0) {
        printf("Error: No valid words found in the file\n");
        freeList(wordList);
        return 1;
    }

    // Get number of guesses from user with input validation
    int maxGuesses = 0;
    int validInput = 0;
    
    while (!validInput) {
        printf("How many total guesses? ");
        
        // Read input as string to handle invalid input properly
        char input[100];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input\n");
            freeList(wordList);
            return 1;
        }

        // Try to convert input to integer
        if (sscanf(input, "%d", &maxGuesses) == 1) {
            if (maxGuesses > 0) {
                validInput = 1;
            } else {
                printf("Error: Number of guesses must be greater than 0\n");
            }
        } else {
            printf("Error: Please enter a valid number\n");
        }
        printf("\n");
    }

    // Start the main game loop with the loaded words and user settings
    play(wordList, wordLength, maxGuesses);

    // Cleanup the word list before exiting
    freeList(wordList);

    return 0;
}