/**
 * @file game.h
 * @brief Header file for Hangman game logic module
 * 
 * Contains function declarations for the main game loop and
 * game state management functions.
 * 
 */

#ifndef GAME_H
#define GAME_H

#include "list.h"

/**
 * @brief Main game loop function
 * 
 * Implements the Hangman game logic where the computer strategically
 * selects word families to make the game unwinnable for the player.
 * 
 * @param wordList Initial list of all possible words
 * @param wordLength Length of each word in the game
 * @param maxGuesses Maximum number of guesses allowed
 */
void play(LIST *wordList, int wordLength, int maxGuesses);

/**
 * @brief Check if the game is won
 * 
 * Determines if the player has successfully guessed all letters
 * in the secret word by checking for underscores in the pattern.
 * 
 * @param pattern Current game pattern with revealed letters
 * @param wordLength Length of the words being guessed
 * @return int 1 if game is won, 0 otherwise
 */
int isGameWon(const char* pattern, int wordLength);

/**
 * @brief Get a valid letter guess from the player
 * 
 * Prompts the player for a letter guess and validates the input.
 * Ensures the guess is a single lowercase letter that hasn't been
 * guessed before.
 * 
 * @param guessedLetters Array tracking which letters have been guessed
 * @param wordLength Length of words (for display purposes)
 * @return char Valid lowercase letter guess
 */
char getPlayerGuess(void);

#endif /* GAME_H */