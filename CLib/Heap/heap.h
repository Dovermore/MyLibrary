/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: heap.h

 *This program is created for the purpose of:
    Header file of heap implementation.
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

/* Header guard */
#ifndef _HEAP_H
#define _HEAP_H


typedef struct heap Heap;

/*
 * return an empty Heap with capacity for max_size items
 */
Heap *new_heap(int max_size);

/*
 * Return current number of elements in the heap
 */
int heap_show_nele(Heap *h);

/*
 * Return current max size of the heap
 */
int heap_show_size(Heap *h);

/*
 * insert data into h with priority key
 */
void heap_insert(Heap *h, float key, int data);

/*
 * Remove and return the item with the smallest key in h
 */
int heap_remove_min(Heap *h);

/*
 * Remove and replace the item with the smallest key in h and return min data
 */
int heap_replace_min(Heap *h, float key, int data);

/*
 * update the priority of a heap item with a particular data value data 
 * while it’s inside the heap.
 */
void heap_change_key(Heap *h, int data, float new_key);

/*
 * return, but not remove, the item with the smallest key in h.
 */
int heap_peek_min(Heap *h);

/*
 * return the key of the item with the smallest key in h
 */
float heap_peek_key(Heap *h);

/*
 * Reset a heap to empty
 */
void heap_reset(Heap *h);


/*
 * de-allocate all memory used by h.
 */
void free_heap(Heap *h);



#endif