/*
 * In this file, you will write the structures and functions needed to
 * implement a hash table.  Feel free to implement any helper functions
 * you need in this file to implement a hash table.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: David Velasco Perez
 * Email:velascod@oregonstate.edu
 */

#include <stdlib.h>
#include <assert.h>


#include "dynarray.h"
#include "list.h"
#include "hash_table.h"


/*
 * This is the structure that represents a hash table.  You must define
 * this struct to contain the data needed to implement a hash table.
 */
// hash table, collision resolution with chaining
struct ht{
    struct dynarray* buckets;
    int num_buckets;
};


/*====================================================================================================*/

/*
 * Function Name: resize
 * Description: This function will resize the hash table. The capacity 
 *              of the hash table and updates the values index according 
 *              to the newly updated capacity
 * Params: 
 *      ht - hash table that will be resized 
 *      convert - will be used to recieve a new hash function index
 * */
void resize(struct ht* ht, int(*convert)(void*)){
    // old_nb keeps track of the old number of buckets
    int old_nb = ht->num_buckets;
    //double size of num buckets
    ht->num_buckets *= 2;    
    
    struct dynarray* new_buckets = dynarray_create();
    /* this first for loop initializes the a new empty hash table */ 
    for (int i = 0; i < ht->num_buckets; i++){
        struct list* empty = list_create();
        dynarray_insert(new_buckets, empty);
    }
    /* 
     * this for loop puts the values of the old hash table and updates index of the values
     * according to new num buckets
     * */
    for (int i = 0; i < old_nb; i++){
        struct list* old_bucket = dynarray_get(ht->buckets, i);
        void* curr = head_get(old_bucket);
        // reindexing and and inserting occurs here
        while(curr){
            int idx = ht_hash_func(ht,node_val(curr), convert);
            struct list* new_b = dynarray_get(new_buckets, idx);
            list_insert(new_b,node_val(curr));
            curr = next_node(curr);
        }
        list_free(old_bucket);
    }
    //free old hash table and sets new hash table

    dynarray_free(ht->buckets);
    ht->buckets = new_buckets;

}
/*====================================================================================================*/


/*
 * This function should allocate and initialize an empty hash table and
 * return a pointer to it.
 */
struct ht* ht_create(){
    struct ht* ht = malloc(sizeof(struct ht));
    ht->buckets = dynarray_create();
    ht->num_buckets = cap(ht->buckets);
    // initialize new hash table with empty list 
    for (int i = 0; i < ht->num_buckets; i++) {
        struct list* bucket = list_create();
        dynarray_insert(ht->buckets, bucket);
    }
    return ht;
}

/*
 * This function should free the memory allocated to a given hash table.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the hash table.  That is the responsibility of the caller.
 *
 * Params:
 *   ht - the hash table to be destroyed.  May not be NULL.
 */
void ht_free(struct ht* ht){
    assert(ht);
    assert(ht->buckets);
    // free all lists at all indexes of dynamic array 
    for(int i = 0; i < ht->num_buckets; i++){ 
        struct list* curr = dynarray_get(ht->buckets, i);
        list_free(curr);
    }
    // free rest 
    dynarray_free(ht->buckets);
    free(ht);
    return;
}

/*
 * This function should return 1 if the specified hash table is empty and
 * 0 otherwise.
 *
 * Params:
 *   ht - the hash table whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if ht is empty and 0 otherwise.
 */
int ht_isempty(struct ht* ht){
    assert(ht);
    return ht_size(ht) == 0; 
}


/*
 * This function returns the size of a given hash table (i.e. the number of
 * elements stored in it, not the capacity).
 *
 * Params:
 *   ht - the hash table whose size is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return the size of the given hash table.
 */
int ht_size(struct ht* ht){
    assert(ht);
    assert(ht->buckets);

    int size = 0;
    //adds all elements(all nodes) at each index of dynarray
    for (int i = 0; i < ht->num_buckets; i++){
        struct list* bucket = dynarray_get(ht->buckets,i);
        size += list_size(bucket);
    }
    return size;
}


/*
 * This function takes a key, maps it to an integer index value in the hash table,
 * and returns it. The hash function is passed in as a function pointer, stored in 'convert'
 * Assuming the passed in hash function is well designed.
 *
 * Params:
 *   ht - the hash table into which to store the element.  May not be NULL.
 *   key - the key of the element used to calculate the index
 *   convert - a pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the index value of 'key' in the hash table .
 */
int ht_hash_func(struct ht* ht, void* key, int (*convert)(void*)){
    assert(ht);

    int hash_code = convert(key);
    return (hash_code % ht->num_buckets); 
}


/*
 * This function should insert a given element into a hash table with a
 * specified key. Note that you cannot have two same keys in one hash table.
 * If the key already exists, update the value associated with the key.  
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * Resolution of collisions is requried, use either chaining or open addressing.
 * If using chaining, double the number of buckets when the load factor is >= 4
 * If using open addressing, double the array capacity when the load factor is >= 0.75
 * load factor = (number of elements) / (hash table capacity)
 *
 * Params:
 *   ht - the hash table into which to insert an element.  May not be NULL.
 *   key - the key of the element
 *   value - the value to be inserted into ht.
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */

void ht_insert(struct ht* ht, void* key, void* value, int (*convert)(void*)){
    assert(ht);
    // idx of key 
    int idx = ht_hash_func(ht, key, convert);
    // get the list at the index of the key
    struct list* bucket = dynarray_get(ht->buckets, idx);
    // inserts the value  
    list_insert_key(bucket, value, key, convert);
    // gets the load factor 
    int num_elem = ht_size(ht);
    float load_factor = num_elem/ht->num_buckets;
    // checks if the load factor is greater than 4
    // if so then a resize is in order
    if(load_factor >= 4.0){
        resize(ht, convert);
    }
    return;
}


/*
 * This function should search for a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, return the corresponding value (void*) of the element,
 * otherwise, return NULL
 *
 * Params:
 *   ht - the hash table into which to loop up for an element.  May not be NULL.
 *   key - the key of the element to search for
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 *
 * Return:
 *   Should return the value of the corresponding 'key' in the hash table .
 */
void* ht_lookup(struct ht* ht, void* key, int (*convert)(void*)){
    
    int idx = ht_hash_func(ht, key, convert);
    struct list* bucket = dynarray_get(ht->buckets, idx);
    
    struct node* curr = head_get(bucket);
    // if the element of the current nodes hash code matches the keys
    // hash code it will return the node
    while(curr){
        if (convert(node_val(curr)) == convert(key)){
            return node_val(curr);
        }
        curr = next_node(curr);
    }
    // if the element is not found, NULL is returned 
    return NULL;
}


/*
 * This function should remove a given element in a hash table with a
 * specified key provided.   
 * This function is passed a function pointer that is used to convert the key (void*) 
 * to a unique hashcode (int). 
 * If the key is found, remove the element and return, otherwise, do nothing and return 
 *
 * Params:
 *   ht - the hash table into which to remove an element.  May not be NULL.
 *   key - the key of the element to remove
 *   convert - pointer to a function that can be passed the void* key from
 *     to convert it to a unique integer hashcode
 */
void ht_remove(struct ht* ht, void* key, int (*convert)(void*)){
    assert(ht);
    
    int idx = ht_hash_func(ht, key, convert);
    // gets the list at the index returned from the hash function
    // and removes the node that matches the hashcode
    struct list *bucket = dynarray_get(ht->buckets,idx);
    return list_remove(bucket, key, convert);


} 
