/*
 * CS 2110 Spring 2017 -> Fall 2017
 * Author: Joshua Reno
 */

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this for my_sbrk */
#include "my_sbrk.h"
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"

#include <stdio.h>

/* You *MUST* use this macro when calling my_sbrk to allocate the
 * appropriate size. Failure to do so may result in an incorrect
 * grading!
 */
#define SBRK_SIZE 2048

/* This is the size of the metadata struct and canary footer in bytes */
#define TOTAL_METADATA_SIZE (sizeof(metadata_t) + sizeof(int))

/* This is the minimum size of a block in bytes, where it can
 * store the metadata, canary footer, and at least 1 byte of data
 */
#define MIN_BLOCK_SIZE (TOTAL_METADATA_SIZE + 1)

/* Used in canary calcuations. See the "Block Allocation" section of the
 * homework PDF for details.
 */
#define CANARY_MAGIC_NUMBER 0xE629

/* Feel free to delete this (and all uses of it) once you've implemented all
 * the functions
 */
#define UNUSED_PARAMETER(param) (void)(param)

/* Our freelist structure - this is where the current freelist of
 * blocks will be maintained. failure to maintain the list inside
 * of this structure will result in no credit, as the grader will
 * expect it to be maintained here.
 * DO NOT CHANGE the way this structure is declared
 * or it will break the autograder.
 */
metadata_t *freelist;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
 * my_calloc() to indicate success or the type of failure. See
 * the definition of the my_malloc_err enum in my_malloc.h for details.
 * Similar to errno(3).
 */
enum my_malloc_err my_malloc_errno;

static void addToList(metadata_t* memory);

/* MALLOC
 * See my_malloc.h for documentation
 */
#define CANARY_CALCULATION(block) ((unsigned int) ((uintptr_t)block ^ CANARY_MAGIC_NUMBER) - block->size)
#define END(block) ((void*) ((uint8_t*)block + block->size))
#define TAIL(block) (*(unsigned int*)((uint8_t*)(END(block)) - sizeof(int)))

static void remove_list(metadata_t* block) {
	metadata_t* curr = freelist;
	metadata_t* prev = NULL;
	while (curr != NULL && block != curr) {
		prev = curr;
		curr = curr->next;
	}
	if (prev != NULL) {
		if (curr != NULL){
			prev->next = curr->next;
		}
	} else {
		freelist = curr->next;
	}
}

static void merge(metadata_t* memory) {
	//printStuff();
	metadata_t* node1 = freelist;
	while (node1 != NULL) {
		if ((uint8_t*)node1 + node1->size == (uint8_t*)memory) {
			//left
			node1->size += memory->size;
			remove_list(memory);
			remove_list(node1);
			addToList(node1);
			memory = node1;
			node1 = freelist;
		} else if ((uint8_t*)memory + memory->size == (uint8_t*)node1) {
			//right
			memory->size += node1->size;
			remove_list(node1);
			remove_list(memory);
			addToList(memory);
			node1 = freelist;
		} else {
			//neither
			node1 = node1->next;
		}
	}
}

static void addToList(metadata_t* memory) {
	if (freelist == NULL) {
		freelist = memory;
		return;
	}
	metadata_t* curr = freelist;
	metadata_t* prev = NULL;
	while (curr != NULL && (memory->size > curr->size)) {
		prev = curr;
		curr = curr->next;
	}
	memory->next = curr;
	if (prev == NULL) {
		freelist = memory;
	} else {
		prev->next = memory;
	}
}

void *my_malloc(size_t size) {
	my_malloc_errno = NO_ERROR;
    if (!size) {return NULL;}

    size_t newSize = size + TOTAL_METADATA_SIZE;

    if (newSize > SBRK_SIZE) {
    	my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
    	return NULL;
    }

    metadata_t* temp = freelist;
    while (temp != NULL && temp->size < newSize) {
    	temp = temp->next;
    }
    // temp is now the smallest block that fits the size
    if (temp == NULL) {
    	temp = my_sbrk(SBRK_SIZE);
    	if (temp == NULL) {
    		my_malloc_errno = OUT_OF_MEMORY;
    		return NULL;
    	}
    	temp->size = SBRK_SIZE;
    	temp->next = NULL;
    	addToList(temp);
    	merge(temp);
    	return my_malloc(size);
    }
    size_t tempSize = MIN_BLOCK_SIZE;
    if (temp->size - newSize > tempSize){
    	//split
    	// remove
    	remove_list(temp);
    	// done with removing, block is the removed
    	metadata_t* block2 = (metadata_t*)((char*)temp + newSize);
    	block2->size = temp->size - newSize;
    	// add and merge
    	addToList(block2);
    	merge(block2);
    	temp->size = newSize;
    	// calculate canary
    	unsigned int canary = CANARY_CALCULATION(temp);
    	temp->canary = canary;
    	TAIL(temp) = canary;
    	my_malloc_errno = NO_ERROR;
    	return temp + 1;
    } else {
    	remove_list(temp);
    	unsigned int canary = CANARY_CALCULATION(temp);
    	temp->canary = canary;
    	TAIL(temp) = canary;
    	my_malloc_errno = NO_ERROR;
    	return temp + 1;
    }
}

/* REALLOC
 * See my_malloc.h for documentation
 */
void *my_realloc(void *ptr, size_t size) {
    if (ptr == NULL) {
    	my_malloc_errno = NO_ERROR;
    	return my_malloc(size);
    }
    if (size == 0) {
    	my_free(ptr);
    	return NULL;
    }
    char* newPtr = my_malloc(size);
    memcpy(newPtr, ptr, size);
    my_free(ptr);
    if (my_malloc_errno == CANARY_CORRUPTED) {
    	my_free(newPtr);
    	my_malloc_errno = CANARY_CORRUPTED;
    	return NULL;
    }
    return newPtr;
}

/* CALLOC
 * See my_malloc.h for documentation
 */
void *my_calloc(size_t nmemb, size_t size) {
    if (!nmemb && !size && (int) size <= 0) {return NULL;}
    size_t newSize = nmemb * size;
    void* pointer = my_malloc(newSize);
    if (pointer == NULL) {return NULL;}
    char* temp = pointer;
    // will this change the value
    while (newSize) {
    	*temp = 0;
    	temp++;
    	newSize--;
    }
    return pointer;
}

/* FREE
 * See my_malloc.h for documentation
 */
void my_free(void *ptr) {
	my_malloc_errno = NO_ERROR;
    if (ptr == NULL) {return;}
    metadata_t* block = (metadata_t*) ((char*) ptr - sizeof(metadata_t));
    unsigned int canary = CANARY_CALCULATION(block);
    if (canary != block->canary || (*(unsigned int*)((uint8_t*)END(block) - sizeof(int))) != canary) {
    	my_malloc_errno = CANARY_CORRUPTED;
    	return;
    }
   	addToList(block);
   	merge(block);
   	my_malloc_errno = NO_ERROR;
   	return;
}