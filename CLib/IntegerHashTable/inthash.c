/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: inthash.c
 
 *Brief:
    Implementation of my hash table for integer
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "inthash.h"
#include "../Item/item.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
#define INVALID -1
#define INIT_SIZE 2
/* </MacroDefs> */


/* <TypeDefs> */
/* Empty */
/* </TypeDefs> */


/* <FunctionPrototypes> */
/* –––––––––––––––––––– */
static int least_pow2(int size);
static int is_chain_full(HashTable *h, int ind);
static void double_table(HashTable *h);
static void double_chain(HashTable *h, int ind);
static int in_chain(Item *chain, int key, int nele);
// static Item *hash_return_item(HashTable *h, int key);
/* </FunctionPrototypes> */



static int least_pow2(int size) {
    int v = 1;
    while (v < size) {
        v *= 2;
    }
    return v;
}

static int is_chain_full(HashTable *h, int ind) {
    // printf("check length %d %d\n", h->nele_chain[ind], h->size_chain[ind]);
    return h->nele_chain[ind] >= h->size_chain[ind];
}

/* TO be implemented */
/* TO be implemented */
/* TO be implemented */
static void double_table(HashTable *h) {
    // Rehash everything
}

static void double_chain(HashTable *h, int ind) {
    int size = h->size_chain[ind] * 2;
    h->size_chain[ind] = size;
    // printf("doubling to %d\n", size);
    h->table[ind] = (Item*)realloc(h->table[ind], size*sizeof*(h->table[ind]));
}

static int in_chain(Item *chain, int key, int nele) {
    for (int i = 0; i < nele; i++) {
        if (chain[i].key == key) {
            item_swap(chain, chain+i);
            return TRUE;
        }
    }
    return FALSE;
}

// static Item *hash_return_item(HashTable *h, int key) {
//     int ind = hash(key, h->size_table);
//     if (in_chain(h->table[ind], key, h->nele_chain[ind])) {
//         return h->table[ind];
//     }
//     return NULL;
// }


HashTable *new_hash_table(int size) {
    assert(size >= 1);
    size = least_pow2(size);

    HashTable *h = (HashTable*)malloc(sizeof(HashTable));
    assert(h != NULL);
    h->size_table = size;

    h->table = (Item**)malloc(size * sizeof*(h->table));
    assert(h->table != NULL);

    h->size_chain = (int*)malloc(size * sizeof*(h->size_chain));
    assert(h->size_chain != NULL);

    h->nele_chain = (int*)malloc(size * sizeof*(h->nele_chain));
    assert(h->nele_chain != NULL);

    for (int i=0; i<size; i++) {
        h->size_chain[i] = INIT_SIZE;
        h->nele_chain[i] = 0;
        h->table[i] = (Item*)malloc(INIT_SIZE*sizeof*(h->table[i]));
        assert(h->table[i] != NULL);
    }
    return h;
}


void free_hash_table(HashTable *h) {
    assert(h!=NULL);

    assert(h->size_chain != NULL);
    free(h->size_chain);

    assert(h->nele_chain != NULL);
    free(h->nele_chain);

    assert(h->table != NULL);
    for (int i=0; i<h->size_table; i++) {
        assert(h->table[i] != NULL);
        free(h->table[i]);
    }
    free(h->table);

    free(h);
}


void hash_insert(HashTable *h, int key, int data) {
    assert(h != NULL);

    int ind = hash(key, h->size_table);

    // printf("Inserting: key: %d data: %d hash: %d\n", key, data, ind);
    if (hash_update_data(h, key, data) != -1) {
        // printf("Finished insert\n");
        return; 
    }
    if (is_chain_full(h, ind)) {
        double_chain(h, ind);
    }

    /* Move the first item to tail of the chain */
    item_swap(h->table[ind], (h->table[ind])+(h->nele_chain[ind]));
    h->table[ind]->data = data;
    h->table[ind]->key = (float)key;
    h->nele_chain[ind]++;
    // printf("Finished insert\n");
}

int hash_check_key(HashTable *h, int key) {
    int ind = hash(key, h->size_table);
    return in_chain(h->table[ind], key, h->nele_chain[ind]);
}

int hash_return_data(HashTable *h, int key) {
    int ind = hash(key, h->size_table);
    if (in_chain(h->table[ind], key, h->nele_chain[ind])) {
        return h->table[ind]->data;
    }
    return INVALID;
}

int hash_remove_key(HashTable *h, int key) {
    int ind  = hash(key, h->size_table);
    if (!in_chain(h->table[ind], key, h->nele_chain[ind])) {
        return INVALID;
    }

    h->nele_chain[ind]--;
    item_swap(h->table[ind], h->table[ind]+(h->nele_chain[ind]));
    return h->table[ind]->data;
}

int hash_update_data(HashTable *h, int key, int data) {
    int ind = hash(key, h->size_table);

    /* if exist, update it and move to front */
    if (in_chain(h->table[ind], key, h->nele_chain[ind])) {
        int old_data = h->table[ind]->data;
        h->table[ind]->data = data;
        return old_data;
    }
    return INVALID;
}


void print_hash_table(HashTable *h, int show) {
    int max = 0, min = 10000000, j = 0;
    Item temp;
    for (int i = 0; i < h->size_table; i++) {
        if (show) printf("slot %d have item: ", i);
        for (j = 0; j < h->nele_chain[i]; j++) {
            temp = h->table[i][j];
            if (show) printf("|key:%5d  data:%5d |", (int)temp.key, temp.data);
        }
        max = j > max? j : max;
        min = j < min? j : min;
        if (show) printf("\n");
        if (!show) printf("length in this slot is %d\n", j);
    }
    printf("max chain %d, min chain %d\n", max, min);
    printf("Finished printing\n");
}

