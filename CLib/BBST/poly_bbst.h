/*****************************************************************************
 *Author          Calvin Zhuoqun Huang
 *Licence         MIT Licence
 *File Name       poly_bbst.h
 *Space for Tab   YES
 *Tab size        4
 *Time Created    30 April 2018 (Monday)
 *Last Edited     30 April 2018 (Monday)

 *Brief:
        General purpose polymorphic AVL tree
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/


/* Header guard */
#ifndef _POLY_BBST_H
#define _POLY_BBST_H


typedef struct tree_s Tree;


/*
 * Create a new tree and return the pointer to the tree
 */
Tree *new_tree(size_t skey, size_t sdata, int (*cmp)(void*, void*));

/*
 * Free the memory allocated to a tree and all it's nodes
 */
void free_tree(Tree *t)

/*
 * Iterative insert (key, data) pair to tree while keeping the tree balanced
 */
void tree_insert(Tree *t, void *key, void *data);

/*
 * Retrieve a single data of given key
 */
void *retrieve_data(Tree* t, void *key);

/*
 * Retrieve all key, data pairs in the tree. Call the empty function to reset
 * the values.
 * The value is retrieved in ascending order.
 */
int retrieve_tree(Tree *t, void *key, void *data);

/* 
 * Return number of elements in the tree
 */
int tree_nele(Tree *t);

/*
 * This is a test use-only interface. MUST use INTEGER for keys,
 * and whatever type for data
 */
void test_tree(Tree *t);

#endif