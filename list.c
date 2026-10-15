/**
 * @file list.c
 * @brief Linked list implementation for word storage
 * 
 * This module implements basic linked list operations used
 * throughout the Hangman program for storing and managing
 * word lists and word families.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/**
 * @brief Create an empty list
 */
LIST *createList(void) {
    LIST *l = malloc(sizeof(LIST));
    if (!l) return NULL;
    l->head = NULL;
    l->size = 0;
    return l;
}

/**
 * @brief Insert copy of word at head of list
 */
void insertList(LIST *l, const char *word) {
    if (!l || !word) return;
    
    NODE *n = malloc(sizeof(NODE));
    if (!n) return;
    
    n->name = malloc(strlen(word) + 1); // Create copy of the string
    if (n->name) {
        strcpy(n->name, word);
    } else {
        free(n);
        return;
    }
    n->next = l->head;      // Insert at head
    l->head = n;
    l->size++;              // Update size counter
}

/**
 * @brief Free whole list and its strings
 */
void freeList(LIST *l) {
    if (!l) return;
    
    NODE *cur = l->head;
    while (cur) {
        NODE *tmp = cur->next;
        free(cur->name); // Free the string copy
        free(cur);       // Free the node itself
        cur = tmp;
    }
    free(l); // Free the list header
}

/**
 * @brief Get number of elements in list
 */
int listSize(const LIST *l) {
    return l ? l->size : 0;
}

/**
 * @brief Return first element's name
 */
const char *listFirst(const LIST *l) {
    if (!l || !l->head) return NULL;
    return l->head->name;
}

#ifdef TEST_LIST
/**
 * @brief Test driver for linked list operations
 */
int main(){
    LIST *list = createList();
    insertList(list, "adore");
    insertList(list, "heloo");
    insertList(list, "hi");

    printf("%d \n", listSize(list));
    
    // List will be automatically freed when program ends
    // In real usage, you should call freeList(list)
}
#endif