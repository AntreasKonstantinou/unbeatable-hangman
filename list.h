/**
 * @file list.h
 * @brief Header file for linked list operations module
 * 
 * Contains structure definitions and function declarations for
 * singly linked list implementation used throughout the program.
 * 
 */

#ifndef LIST_H
#define LIST_H

/**
 * @brief Node structure for linked list
 * 
 * Represents a single node in the singly linked list,
 * containing a string and a pointer to the next node.
 */
typedef struct node {
    char *name;         /**< String data stored in the node */
    struct node *next;  /**< Pointer to the next node in the list */
} NODE;

/**
 * @brief List structure header
 * 
 * Maintains the head pointer and size of the linked list
 * for efficient access and size tracking.
 */
typedef struct list {
    NODE *head; /**< Pointer to the first node in the list */
    int size;   /**< Number of nodes in the list */
} LIST;

/** 
 * @brief Create an empty list
 * 
 * Allocates and initializes a new empty linked list structure.
 * 
 * @return LIST* Pointer to newly created list, NULL on failure
 */
LIST *createList(void);

/** 
 * @brief Insert copy of word at head of list
 * 
 * Creates a new node containing a copy of the given string
 * and inserts it at the beginning of the list.
 * 
 * @param l List to insert into
 * @param word String to copy and insert
 */
void insertList(LIST *l, const char *word);

/** 
 * @brief Free whole list and its strings
 * 
 * Completely deallocates the list structure and all nodes,
 * including the string data stored in each node.
 * 
 * @param l List to free
 */
void freeList(LIST *l);

/** 
 * @brief Get number of elements in list
 * 
 * Returns the current size of the list maintained in the
 * list structure for efficient access.
 * 
 * @param l List to get size of
 * @return int Number of elements in the list
 */
int listSize(const LIST *l);

/** 
 * @brief Return first element's name
 * 
 * Provides access to the string data in the first node
 * of the list without removing it.
 * 
 * @param l List to get first element from
 * @return const char* String in first node, NULL if list is empty
 */
const char *listFirst(const LIST *l);

#endif /* LIST_H */