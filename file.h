/**
 * @file file.h
 * @brief Header file for file operations module
 * 
 * Contains function declarations for reading word files and
 * detecting word lengths.
 * 
 */

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include "list.h"

/** 
 * @brief Copy words from file into list
 * 
 * Reads first number as word length, then reads all words of that length
 * into the linked list. Each word must match the specified length.
 * 
 * @param file File pointer to read from
 * @param list Linked list to store words in
 */
void copyFile(FILE *file, LIST *list);

/** 
 * @brief Get word length from file
 * 
 * Reads the first integer from the file which represents the length 
 * of each word in the file.
 * 
 * @param file File pointer to read from
 * @return int Word length, or -1 if error
 */
int wordSize(FILE *file);

#endif /* FILE_H */