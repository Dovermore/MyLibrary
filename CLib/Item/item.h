/* Header guard */
#ifndef _ITEM_H
#define _ITEM_H


// typedef for a simple item type

typedef struct item {
    int data;
    float key;
} Item;

/*
 * Swap the position of two Item object with pointer
 */
void item_swap(Item *a, Item *b);


#endif