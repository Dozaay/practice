/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name:David Velasco Perez
 * Email:velascod@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq{
    struct dynarray* elem;
    struct dynarray* priority;
};

/*====================================================================================================*/
// min helper function
int min(int a, int b){
    return (a < b)? a : b;
}

// helper function to help me reorder the heap after inserting.

/*====================================================================================================*/

/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
    struct pq* pq = malloc(sizeof(struct pq));
    pq->elem = dynarray_create();	
    pq->priority = dynarray_create();
    return pq;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue. That is the responsibility of the caller. However, you
 * do need to free the memory that holds each element stored in the priority 
 * queue. 
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	assert(pq);
    assert(pq->elem);
    assert(pq->priority);
    dynarray_free(pq->elem);
    dynarray_free(pq->priority);
    free(pq);
    return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
    assert(pq);
    if (dynarray_isempty(pq->elem) && dynarray_isempty(pq->priority)) {
        return 1;
    } else {
        return 0;
    }
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
    assert(pq);
    dynarray_insert(pq->elem, value);
    dynarray_insert(pq->priority, (void*)(intptr_t)priority);
    heap_up(pq->elem, pq->priority, dynarray_size(pq->elem) - 1);
    return;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
    assert(pq);
    assert(pq->elem);
    if (!dynarray_isempty(pq->elem)){
        return first(pq->elem);
    }
    return NULL;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
    assert(pq);
    assert(pq->priority);
    if (!dynarray_isempty(pq->elem)){
        return (int)(intptr_t)first(pq->priority);
    }
    return -1;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	assert(pq);
    assert(!dynarray_isempty(pq->elem));
    assert(!dynarray_isempty(pq->priority));
    void *Felem = pq_first(pq);	
    H_rm_first(pq->elem, pq->priority);

    return Felem;
} 
