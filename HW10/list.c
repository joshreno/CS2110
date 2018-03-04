/**
 * CS 2110 - Fall 2017 - Homework #10
 *
 * @author Joshua Reno
 *
 * list.c: Complete the functions!
 */

/**
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!-IMPORTANT-!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * For any function that must use malloc, if malloc returns NULL, the function
 * itself should return NULL if needs to return something (or return false if
 * the function returns a bool).
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

// This is just to remove compiler warnings associated with unused variables.
// Delete calls to this as you implement functions.
#define UNUSED_PARAMETER(x) (void)(x)

/* The node struct. Has a next pointer, and data. */
/* DO NOT MODIFY THIS STRUCTURE */
/* Only this file should deal with the node struct */
typedef struct _list_node
{
    struct _list_node *next;
    void *data;
} node;

/* You should NOT have any global variables. */

/* The create_node function is static because this is the only file that should
   have knowledge about the nodes backing the linked list. */
static node *create_node(void *data);

/** create_node
  *
  * Helper function that creates a node by allocating memory for it on the heap.
  * Be sure to set its next pointer to NULL.
  *
  * If malloc returns NULL, you should return NULL to indicate failure.
  *
  * @param data a void pointer to data the user wants to store in the list
  * @return a node
  */
static node* create_node(void *data)
{
    node* temp = malloc(sizeof(node));
    if (temp == NULL) {
      return NULL;
    }
    temp->data = data;
    temp->next = NULL;
    return temp;
}

/** create_list
  *
  * Creates a list by allocating memory for it on the heap.
  * Be sure to initialize size to zero and head and tail to NULL.
  *
  * If malloc returns NULL, you should return NULL to indicate failure.
  *
  * @return a pointer to a new list or NULL on failure
  */
list *create_list(void)
{
    list* temp = malloc(sizeof(list));
    if (temp == NULL) {
      return NULL;
    }
    temp->head = NULL;
    temp->size = 0;
    temp->tail = NULL;
    return temp;
}

/** shallow_copy_list
  *
  * Creates a shallow copy of the list. This means a new list structure and
  * nodes should be created, but each node will point to the same data as the
  * list passed in.
  *
  * if malloc fails or the list is NULL, return NULL to indicate failure. If
  * any memory allocations failures occur, your code must not leak memory. That
  * is, if create_node() returns NULL, you must go back and free any nodes
  * you've allocated so far and the new list.
  *
  * @param listToCopy the list to shallow copy
  * @return a pointer to the new list or NULL on failure
  */
list *shallow_copy_list(list *listToCopy)
{
    if (listToCopy == NULL) {
      return NULL;
    }
    list* nList = create_list();
    if (nList == NULL) {
      free(nList);
      return NULL;
    }
    node* node = listToCopy->head;
    int index = 0;
    while (node != NULL) {
      add(nList, index, node->data);
      node = node->next;
      index++;
    }
    return nList;
}

/** deep_copy_list
  *
  * Create a new list structure, new nodes, and new copies of the data by using
  * the copy function. Its implementation for any structure must copy
  * EVERYTHING!
  *
  * If listToCopy or copyFunc is NULL, return NULL. For memory allocations
  * failures, including if copyFunc() returns false (indicating a memory
  * allocation failure), your code must not leak memory. This means that if any
  * memory allocation failures occur, before you return NULL, you must go back
  * and free all data in the new list, nodes in the new list, and the new list
  * itself.
  *
  * @param listToCopy A pointer to the list structure to make a copy of
  * @param copyFunc A function pointer to a function that makes a copy of the
  *                 data that's being used in this list, allocating space for
  *                 every part of that data on the heap. This is some function
  *                 you must write yourself for testing, tailored specifically
  *                 to whatever context you're using the list for in your test.
  *                 If copyFunc returns false for any element, deep_copy_list()
  *                 should return NULL.
  * @param freeFunc function used to free the nodes' data in the case of a
  *                 memory allocation failure when copying (see above)
  * @return The list structure created by copying the old one, or NULL on
  *         failure
  */
list *deep_copy_list(list *listToCopy, list_copy copyFunc, list_op freeFunc)
{
    if (listToCopy == NULL || copyFunc == NULL || freeFunc == NULL) {
      return NULL;
    }
    list* nList = create_list();
    if (nList == NULL) {
      return NULL;
    }
    node* nNode = listToCopy->head;
    void* temp;
    while (nNode != NULL) {
      int result = copyFunc(nNode->data, &temp);
      if (result == 0) {
        empty_list(nList, freeFunc);
        free(nList);
        return NULL;
      }
      push_back(nList, temp);
      nNode = nNode->next;
    }
    //free(temp);
    return nList;


}

/** push_front
  *
  * Adds the element at the front of the list.
  *
  * @param listToPrependTo a pointer to the list structure.
  * @param data the data to add to the list.
  * @return false if the list is NULL, true if successful.
  */
bool push_front(list *listToPrependTo, void *data)
{
    // if (data == NULL) {
    //   return 0;
    // }
    bool result = add(listToPrependTo, 0, data);
    return result;
}

/** push_back
  *
  * Adds the element to the back of the list.
  *
  * @param listToAppendTo a pointer to the list structure.
  * @param data the data to add to the list.
  * @return false if the list is NULL, true if successful.
  */
bool push_back(list *listToAppendTo, void *data)
{
    // if (data == NULL || listToAppendTo == NULL) {
    //   return 0;
    // }
    if (listToAppendTo == NULL) {
      return 0;
    }
    bool result = add(listToAppendTo, listToAppendTo->size, data);
    return result;
}

/** add
  *
  * Add the element at the specified index in the list. This index must lie in
  * the inclusive range [0,size].
  * For example, if you have no elements in the list,
  * you should be able to add to index 0 but no further.
  * If you have two elements in the list,
  * you should be able to add to index 2 but no further.
  *
  * @param listToAddTo a pointer to the list structure
  * @param index 0-based, starting from the head in the inclusive range
  *              [0,size]
  * @param data the data to add to the list
  * @return false if the index is out of bounds or the list is NULL
  *         (do not add the data in this case)
  *         otherwise return true
  */
bool add(list *listToAddTo, int index, void *data)
{
    if (listToAddTo == NULL) {
      return 0;
    }
    if (index < 0  || index > listToAddTo->size) {
      return 0;
    }
    // if (data == NULL) {
    //   return 0;
    // }
    int count = 0;
    node *temp = listToAddTo->head;
    node *newNode = create_node(data);
    if (newNode == NULL) {
      free(newNode);
      return 0;
    }
    if (index == 0) {
      //printf("WORKSISH\n");
      newNode->next = temp;
      listToAddTo->head = newNode;
      if (listToAddTo->size == 0) {
        listToAddTo->tail = newNode;
      }
      if (listToAddTo->size == 1) {
        listToAddTo->tail = temp;
      }
      listToAddTo->size += 1;
      return 1;
    }
    while (count < index - 1) {
      temp = temp->next;
      count++;
    }
    if (listToAddTo->size == 0) {
      listToAddTo->head = newNode;
      listToAddTo->tail = newNode;
      //printf("WORKS1\n");
    } else if (index == listToAddTo->size) {
      //printf("WORKS2\n");
      newNode->next = NULL;
      temp->next = newNode;
      listToAddTo->tail = newNode;
    } else {
      //printf("WORKS3\n");
      newNode->next = temp->next;
      temp->next = newNode;
      // if (index == listToAddTo->size) {
      //   printf("WORKS\n");
      //   listToAddTo->tail = newNode->next;
      // }
    }

    listToAddTo->size += 1;
    return 1;
}

/** front
  *
  * Gets the data at the front of the list
  * If the list is empty or the list is NULL, return false.
  *
  * @param listToGetFrontOf a pointer to the list
  * @param dataOut A pointer to a pointer used to return the data at the first
  *                node in the list or NULL on failure.
  * @return false if dataOut is NULL or the list is NULL or empty, true if
  *         successful.
  */
bool front(list *listToGetFrontOf, void **dataOut)
{
    bool result = get(listToGetFrontOf, 0, dataOut);
    return result;
}

/** back
  *
  * Gets the data at the back of the list
  * If the list is empty or the list is NULL, return false.
  *
  * @param listToGetBackOf a pointer to the list
  * @param dataOut A pointer to a pointer used to return the data at the last
  *                node in the list or NULL on failure.
  * @return false if dataOut is NULL or the list is NULL or empty, true
  *         otherwise
  */
bool back(list *listToGetBackOf, void **dataOut)
{
    if (listToGetBackOf == NULL) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    bool result = get(listToGetBackOf, listToGetBackOf->size - 1, dataOut);
    return result;
}

/** get
  *
  * Gets the data at the specified index in the list
  *
  * @param listToGetDataFrom a pointer to the list structure
  * @param index 0-based, starting from the head.
  * @param dataOut A pointer to a pointer used to return the data from the
  *        specified index in the list or NULL on failure.
  * @return false if dataOut is NULL or index is out of range of the list or
  *         the list is NULL, true otherwise
  */
bool get(list *listToGetDataFrom, int index, void **dataOut)
{
    if (listToGetDataFrom == NULL) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    if (dataOut == NULL) {
      return 0;
    }
    if (index < 0  || index > listToGetDataFrom->size) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    if (listToGetDataFrom->size == 0) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    if (index == listToGetDataFrom->size) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    int count = 0;
    node* temp = listToGetDataFrom->head;
    while (count < index) {
      temp = temp->next;
      count++;
    }
    *dataOut = temp->data;
    return 1;
}

/** size
  *
  * Gets the size of the list
  *
  * if the list is NULL, return false.
  *
  * @param listToGetSizeOf a pointer to the list structure
  * @param A pointer used to return the size of the list, set to -1 on NULL list
  * @return false if sizeOut is NULL or list is NULL, true otherwise
  */
bool size(list *listToGetSizeOf, int *sizeOut)
{
    if (sizeOut == NULL) {
      return 0;
    }
    if (listToGetSizeOf == NULL) {
      *sizeOut = -1;
      return 0;
    }
    *sizeOut = listToGetSizeOf->size;
    return 1;
}

/** contains
  *
  * Traverses the list, trying to see if the list contains some data.
  *
  * The "data" parameter may not necessarily point to the same address as the
  * equal data you are returning from this function, it's just data which the
  * comparator says is equal. For instance, if you have a list of person structs:
  *   (Andrew, 26), (Nick, 24), (Collin, 23), (Marie, 23)
  * And you want to return any person whose age is 24, you could create a new
  * person struct (<NULL>, 24) with a comparator that returns 0 if age == 24 and
  * pass that into this function as "data" along with comparator. contains()
  * would then return (Nick, 24) In other words, data will be something with
  * value equality to what is in the list, but not necessarily something with
  * reference equality. Value equality will be defined by comparator.
  *
  * If there are multiple pieces of data in the list which are equal to the
  * "data" parameter, return any one of them.
  *
  * @param listToSearch a pointer to the list structure
  * @param data The data, to see if it exists in the list
  * @param comparator A function written by the user that will tell if two pieces
  *                of data are equal. Returns 0 if equal, something else
  *                otherwise. Imagine subtracting two numbers. Think of
  *                a comparator's compare function from Java.
  * @param dataOut A pointer to a pointer used to return the data contained in
  *                the list or NULL on failure
  * @return false if dataOut is NULL, list is NULL, comparator is NULL, or the
  *               list does not contain data, else true
  */
bool contains(list *listToSearch, void *data, list_eq comparator, void **dataOut)
{
    if (dataOut == NULL || comparator == NULL || listToSearch == NULL || listToSearch->size == 0) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    node* temp = listToSearch->head;
    for (int i = 0; i < listToSearch->size; i++) {
      if (comparator(data, temp->data) == 0) {
        *dataOut = temp->data;
        return 1;
      }
      temp = temp->next;
    }
    *dataOut = NULL;
    //dataOut = NULL;
    return 0;
}


/** pop_front
  *
  * Removes the node at the front of the list, and returns its data to the user
  *
  * @param listToPopFrom a pointer to the list.
  * @param dataOut A pointer to a pointer used to return the data in the first
  *                node or NULL on failure
  * @return false if dataOut is NULL or the list is NULL or empty, else true
  */
bool pop_front(list *listToPopFrom, void **dataOut)
{

    bool result = list_remove(listToPopFrom, 0, dataOut);

    return result;
}

/** pop_back
  *
  * Removes the node at the back of the list, and returns its data to the user
  *
  * @param listToPopFrom a pointer to the list.
  * @param dataOut A pointer to a pointer used to return the data in the last
  *                node or NULL on failure
  * @return false if dataOut is NULL or the list is NULL or empty, else true
  */
bool pop_back(list *listToPopFrom, void **dataOut)
{
    if (listToPopFrom == NULL) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    bool result = list_remove(listToPopFrom, listToPopFrom->size - 1, dataOut);
    return result;
}

/** list_remove
  *
  * Removes the data from the specified index in the list
  *
  * @param listToRemoveFrom a pointer to the list structure
  * @param index 0-based, starting from the head.
  * @param dataOut A pointer to a pointer used to return the data from the
  *                specified index in the list or NULL on failure
  * @return false if dataOut is NULL or list is NULL or index is out of range
  *         of the list, else true
  */
bool list_remove(list *listToRemoveFrom, int index, void **dataOut)
{
    if (listToRemoveFrom == NULL || listToRemoveFrom->size == 0) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    if (index < 0  || index > listToRemoveFrom->size) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    if (dataOut == NULL) {
      return 0;
    }
    if (index == listToRemoveFrom->size) {
      if (dataOut != NULL) {
        *dataOut = NULL;
      }
      return 0;
    }
    int count = 0;
    node* temp = listToRemoveFrom->head;
    while (count < index - 1) {
      temp = temp->next;
      count++;
    }
    if (listToRemoveFrom->size == 1) {
      *dataOut = temp->data;
      free(temp);
      listToRemoveFrom->head = NULL;
      listToRemoveFrom->tail = NULL;
    } else if (index == 0) {
      // temp is the head;
      listToRemoveFrom->head = temp->next;
      *dataOut = temp->data;
      free(temp);
    } else {
      node *next = temp->next;
      node *secondNext = next->next;
      if (index + 1 == listToRemoveFrom->size) {
        listToRemoveFrom->tail = secondNext;
      }

      *dataOut = next->data;
      //printf("%p\n", *dataOut);

      temp->next = secondNext;
      // what absout the data;
      free(next);
    }
    listToRemoveFrom->size = listToRemoveFrom->size - 1;
    // temp->data = data;
    // return 1;mp->data = data;
    // need to create_node
    return 1;
}


/** empty_list
  *
  * Empties the list. After this is called, the list should be empty.
  * This does not free the list struct itself, just all nodes and data within.
  * Make sure to check that listToEmpty and freeFunc are not NULL before using
  * them.
  *
  * @param listToEmpty a pointer to the list structure
  * @param freeFunc function used to free the nodes' data.
  */
void empty_list(list *listToEmpty, list_op freeFunc)
{
    if (listToEmpty == NULL || freeFunc == NULL) {
      return;
    }
    // node* temp1 = listToEmpty->head;
    // node* temp2 = temp1->next;
    // for (int i = 0; i < listToEmpty->size - 1; i++) {
    //   freeFunc(temp1->data);
    //   temp1 = temp2;
    //   temp2 = temp1->next;
    //   free(temp1);
    // }
    // listToEmpty->head = NULL;
    // listToEmpty->tail = NULL;
    // listToEmpty->size = 0;
    node* temp1 = listToEmpty->head;
    node* temp2 = listToEmpty->head;
    for (int i = 0; i < listToEmpty->size; i++) {
      temp2 = temp2->next;
      freeFunc(temp1->data);
      free(temp1);
      temp1 = temp2;
    }
    listToEmpty->head = NULL;
    listToEmpty->tail = NULL;
    listToEmpty->size = 0;
}

/** traverse
  *
  * Traverses the list, calling a function on each node's data.
  *
  * @param listToTraverse a pointer to the list structure
  * @param doFunc a function that does something to each node's data.
  * @return true if the traversal was a success, false otherwise
  *         (listToTraverse1 or doFunc is null)
  */
bool traverse(list *listToTraverse, list_op doFunc)
{
    if (listToTraverse == NULL || doFunc == NULL) {
      return 0;
    }
    node* temp = listToTraverse->head;
    for (int i = 0; i < listToTraverse->size; i++) {
      doFunc(temp->data);
      temp = temp->next;
    }
    return 1;
}

/** reverse
  *
  * Reverses the singly linked list in place.
  *
  * @param listToReverse to append to.
  * @return true if the operation was a success, false otherwise (listToReverse
  *         was NULL)
  */
bool reverse(list *listToReverse)
{
    if (listToReverse == NULL) {
      return 0;
    }
    if (listToReverse->size == 0) {
      return 1;
    }
    if (listToReverse->size == 1) {
      return 1;
    }
    node* currentNode = listToReverse->head;
    listToReverse->tail = currentNode;
    node* nextNode = NULL;
    node* previousNode = NULL;
    while (currentNode != NULL) {
      nextNode = currentNode->next;
      currentNode->next = previousNode;
      previousNode = currentNode;
      currentNode = nextNode;
    }
    listToReverse->head = previousNode;
    return 1;
}

/** concat
  *
  * Concatinates firstList and secondList. This function should append
  * secondList to the end of firstList.
  *
  * NOTE: this function will effectively empty lastList onto firstList which
  *       means that lastList will be empty (head == tail == NULL and size == 0)
  *       at the end of the function and firstList will contain all the elements
  *       from both lists. Be sure to update the size, head, and tail pointers
  *       accordingly.
  *
  * example: if firstList is a->b->c and second list is d->e->f
  * after the function call, firstList will be a->b->c->d->e->f and
  * secondList will be empty.
  *
  *
  * @param firstList to append to.
  * @param secondList the list to append to firstList
  * @return true if the operation was a success, false otherwise (firstList or
  *         secondList is null)
  */
bool concat(list *firstList, list *lastList)
{
    if (firstList == NULL) {
      return 0;
    }
    if (lastList == NULL) {
      // I think - not sure
      return 0;
    }
    if (firstList->size == 0) {
      firstList->head = lastList->head;
      firstList->tail = lastList->tail;
      firstList->size = lastList->size;
      lastList->head = NULL;
      lastList->tail = NULL;
      lastList->size = 0;
      return 1;
    }
    node* firstTail = firstList->tail;
    //printf("%i\n", (firstTail == NULL));
    node* lastHead = lastList->head;
    //printf("%i\n", (firstTail == NULL));
    firstTail->next = lastHead;
    // now need to set the tail properly and clear
    node* lastTail = lastList->tail;
    firstList->tail = lastTail;
    lastList->head = NULL;
    lastList->tail = NULL;
    firstList->size += lastList->size;
    lastList->size = 0;
    //free(lastList);`
    return 1;
}
