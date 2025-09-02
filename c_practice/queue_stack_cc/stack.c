/*
 * This file is where you should implement your stack.  It already contains
 * skeletons of the functions you need to implement (along with documentation
 * for each function).  Feel free to implement any additional functions you
 * might need.  Also, don't forget to include your name and @oregonstate.edu
 * email address below.
 *
 * Name:David Velasco Perez
 * Email:velascod@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "stack.h"
#include "list.h"

/*
 * This is the structure that will be used to represent a stack.  This
 * structure specifically contains a single field representing a linked list
 * that should be used as the underlying data storage for the stack.
 *
 * You should not modify this structure.
 */
struct stack {
     struct list* list;
};

/*
 * This function should allocate and initialize a new, empty stack and return
 * a pointer to it.
 */
struct stack* stack_create() {
    // allcoate memory for new stack
    struct stack* new_stack = malloc(sizeof(struct stack));
    // create a new list
    struct list* new_list = list_create();
    // the list being pointed by new stack will be set to the list we created
    new_stack->list = new_list; 
    return new_stack;

}

/*
 * This function should free the memory associated with a stack.  While this
 * function should up all memory used in the stack itself, it should not free
 * any memory allocated to the pointer values stored in the stack.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   stack - the stack to be destroyed.  May not be NULL.
 */
void stack_free(struct stack* stack) {
    // check if stack is NULL
    assert(stack);
    // frees the list being pointed to by stack 
    list_free(stack->list);
    // finally free stack
    free(stack); 
   
    
    return;
}

/*
 * This function should indicate whether a given stack is currently empty.
 * Specifically, it should return 1 if the specified stack is empty (i.e.
 * contains no elements) and 0 otherwise.
 *
 * Params:
 *   stack - the stack whose emptiness is being questioned.  May not be NULL.
 */
int stack_isempty(struct stack* stack) {
    
    // checks if stack is NULL
    assert(stack);
    // checks whether list is empty or not
    int empty_or_not = list_empty(stack->list);
    // if empty return 1, if not empty return 0
    return empty_or_not;

}

/*
 * This function should push a new value onto a given stack.  The value to be
 * pushed is specified as a void pointer.  This function must have O(1)
 * average runtime complexity.
 *
 * Params:
 *   stack - the stack onto which a value is to be pushed.  May not be NULL.
 *   val - the value to be pushed.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void stack_push(struct stack* stack, void* val) {
    // inserting val at the front of the list makes it easier to 
    // get a runtime complexity of O(1)  while still following the
    // LIFO rule.

    list_insert(stack->list, val);

    return;
}

/*
 * This function should return the value stored at the top of a given stack
 * *without* removing that value.  This function must have O(1) average runtime
 * complexity.
 *
 * Params:
 *   stack - the stack from which to query the top value.  May not be NULL.
 */
void* stack_top(struct stack* stack){ 
    //assign value of the first node to temp
    void* temp = head_value(stack->list);
    //return value of first node
    return temp;
}

/*
 * This function should pop a value from a given stack and return the popped
 * value.  This function must have O(1) average runtime complexity.
 *
 * Params:
 *   stack - the stack from which a value is to be popped.  May not be NULL.
 *
 * Return:
 *   This function should return the value that was popped.
 */
void* stack_pop(struct stack* stack) {
    //store the popped value in temp so that we can return it later
    void *temp = head_value(stack->list);
    // pops top value 
    rm_front(stack->list);
    // return popped val
    return temp;
}
