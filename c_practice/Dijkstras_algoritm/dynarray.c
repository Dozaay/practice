/*
 * This file contains a simple implementation of a dynamic array.  See the
 * documentation below for more information on the individual functions in
 * this implementation.
 */

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

#include "dynarray.h"

/*
 * This structure is used to represent a single dynamic array.
 */
struct dynarray {
  void** data;
  int size;
  int capacity;
};

#define DYNARRAY_INIT_CAPACITY 8

/*
 * This function allocates and initializes a new, empty dynamic array and
 * returns a pointer to it.
 */
struct dynarray* dynarray_create() {
  struct dynarray* da = malloc(sizeof(struct dynarray));
  assert(da);

  da->data = malloc(DYNARRAY_INIT_CAPACITY * sizeof(void*));
  assert(da->data);
  da->size = 0;
  da->capacity = DYNARRAY_INIT_CAPACITY;

  return da;
}

/*
 * This function frees the memory associated with a dynamic array. Freeing
 * any memory associated with values stored in the array is the responsibility
 * of the caller. 
 *
 * Params:
 *   da - the dynamic array to be destroyed.  May not be NULL.
 */
void dynarray_free(struct dynarray* da) {
  assert(da);
  free(da->data);
  free(da);
}

/*
 * This function returns the size of a given dynamic array (i.e. the number of
 * elements stored in it, not the capacity).
 */
int dynarray_size(struct dynarray* da) {
  assert(da);
  return da->size;
}


/*
 * Auxilliary function to perform a resize on a dynamic array's underlying
 * storage array.
 */
void _dynarray_resize(struct dynarray* da, int new_capacity) {
  assert(new_capacity > da->size);

  /*
   * Allocate space for the new array.
   */
  void** new_data = malloc(new_capacity * sizeof(void*));
  assert(new_data);

  /*
   * Copy data from the old array to the new one.
   */
  for (int i = 0; i < da->size; i++) {
    new_data[i] = da->data[i];
  }

  /*
   * Put the new array into the dynarray struct.
   */
  free(da->data);
  da->data = new_data;
  da->capacity = new_capacity;
}

/*
 * This function inserts a new value to a given dynamic array.  The new element
 * is always inserted at the *end* of the array.
 *
 * Params:
 *   da - the dynamic array into which to insert an element.  May not be NULL.
 *   val - the value to be inserted.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_insert(struct dynarray* da, void* val) {
  assert(da);

  /*
   * Make sure we have enough space for the new element.  Resize if needed.
   */
  if (da->size == da->capacity) {
    _dynarray_resize(da, 2 * da->capacity);
  }

  /*
   * Put the new element at the end of the array.
   */
  da->data[da->size] = val;
  da->size++;
}

/*
 * This function removes an element at a specified index from a dynamic array.
 * All existing elements following the specified index are moved forward to
 * fill in the gap left by the removed element.
 *
 * Params:
 *   da - the dynamic array from which to remove an element.  May not be NULL.
 *   idx - the index of the element to be removed.  The value of `idx` must be
 *     between 0 (inclusive) and n (exclusive), where n is the number of
 *     elements stored in the array.
 */
void dynarray_remove(struct dynarray* da, int idx) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  /*
   * Move all elements behind the one being removed forward one index,
   * overwriting the element to be removed in the process.
   */
  for (int i = idx; i < da->size - 1; i++) {
    da->data[i] = da->data[i+1];
  }

  da->size--;
}

/*
 * This function returns the value of an existing element in a dynamic array.
 *
 * Params:
 *   da - the dynamic array from which to get a value.  May not be NULL.
 *   idx - the index of the element whose value should be returned.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 */
void* dynarray_get(struct dynarray* da, int idx) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  return da->data[idx];
}

/*
 * This function updates (i.e. overwrites) the value of an existing element in
 * a dynamic array.
 *
 * Params:
 *   da - the dynamic array in which to set a value.  May not be NULL.
 *   idx - the index of the element whose value should be updated.  The value
 *     of `idx` must be between 0 (inclusive) and n (exclusive), where n is the
 *     number of elements stored in the array.
 *   val - the new value to be set.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void dynarray_set(struct dynarray* da, int idx, void* val) {
  assert(da);
  assert(idx < da->size && idx >= 0);

  da->data[idx] = val;
}



/*===================================================================================================*/
/*
 * Function name: dynarray_isempty
 * Description: This function checks if the dynamic array is empty.
 * Params:
 *   - da: pointer to the dynamic array
 * Returns: 1 if the array is empty, 0 otherwise
 */
int dynarray_isempty(struct dynarray* da){
    return (da->size == 0 ? 1 : 0);
}

/*
 * Function name: first
 * Description: This function returns the first element of the dynamic array.
 * Params:
 *   - da: pointer to the dynamic array
 * Returns: pointer to the first element of the array
 */
void* first(struct dynarray* da){
    return da->data[0];
}

/*
 * Function name: parent
 * Description: This function calculates the parent index of a given index in a heap.
 * Params:
 *   - i: index of the current element
 * Returns: index of the parent element
 */
int parent(int i){
    return (i-1)/2;
}

/*
 * Function name: _left
 * Description: This function calculates the left child index of a given index in a heap.
 * Params:
 *   - i: index of the current element
 * Returns: index of the left child element
 */
int _left(int i){
    return (2*i)+1;
}

/*
 * Function name: _right
 * Description: This function calculates the right child index of a given index in a heap.
 * Params:
 *   - i: index of the current element
 * Returns: index of the right child element
 */
int _right(int i){
    return (2*i)+2;
} 

/*
 * Function name: _priority
 * Description: This function retrieves the priority value of an element at a given index in the dynamic array.
 * Params:
 *   - da: pointer to the dynamic array
 *   - idx: index of the element
 * Returns: priority value of the element
 */
int _priority(struct dynarray* da, int idx){
    int p = (int)(intptr_t)da->data[idx];
    return p;
}

/*
 * Function name: heap_up
 * Description: This function performs the heap-up operation to maintain the heap property.
 * Params:
 *   - da: pointer to the dynamic array containing the elements
 *   - priority: pointer to the dynamic array containing the priority values
 *   - i: index of the current element
 * Returns: void
 */
void heap_up(struct dynarray* da,struct dynarray* priority, int i){
    // will run until i is equal to 0
    while(i > 0){
        int p = parent(i);
        // checks if priority of of i is less that at
        // parent, if so is value will move up to parent,
        // parents value will move down to i (swap places)
        if(_priority(priority,i) < _priority(priority,p)){
            //value will move up
            void* temp = da->data[i];
            da->data[i] = da->data[p];
            da->data[p] = temp;
            
            //priority value value will move up
            void* temp_p = priority->data[i];
            priority->data[i] =  priority->data[p];
            priority->data[p] = temp_p;
            
            // decrement i to value of parent of node
            i = p;
            
        }else{
            break;
        }
    }
    return;
}

/*
 * Function name: heap_down
 * Description: This function performs the heap-down operation to maintain the heap property.
 * Params:
 *   - elem: pointer to the dynamic array containing the elements
 *   - priority: pointer to the dynamic array containing the priority values
 *   - i: index of the current element
 * Returns: void
 */
void heap_down(struct dynarray* elem,struct dynarray* priority, int i){
    int left = 0;
    int right = 0;
    int size = dynarray_size(elem);
    // runs until left index is equal to size 
    while (_left(i) < size){
        // values of left and right child at i will change until while loop is broken
        left =  _left(i);
        right = _right(i);
        
        // checks for smallest priority value
        int smallest = left;
        if(right < size && _priority(priority, right) < _priority(priority, left) ){
            smallest = right;
        }
        // if priority at i is greater then smallest priority of left or right child
        // values will swap like in heap_up
        if(_priority(priority, i)  > _priority(priority, smallest)){
            void* temp = elem->data[i];
            elem->data[i] = elem->data[smallest];
            elem->data[smallest] = temp;

            void *temp_p = priority->data[i];
            priority->data[i] = priority->data[smallest];
            priority->data[smallest] = temp_p;

            i = smallest;
        }else{
            break;
        }
    }
}

/*
 * Function name: H_rm_first
 * Description: This function removes the first element (root) from the heap and maintains the heap property.
 * Params:
 *   - elem: pointer to the dynamic array containing the elements
 *   - priority: pointer to the dynamic array containing the priority values
 * Returns: void
 */
void H_rm_first(struct dynarray* elem, struct dynarray* priority){
    //replace the root with last element
    elem->data[0] = elem->data[dynarray_size(elem)-1];
    priority->data[0] = priority->data[dynarray_size(priority)-1];
    // decrease size by 1 
    elem->size--;
    priority->size--;
    heap_down(elem, priority, 0);

}


/*===================================================================================================*/
