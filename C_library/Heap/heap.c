/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: heap.c
 
 *This program is created for the purpose of:
    Actual c file of heap
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "heap.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
#define INVALID -1
/* </MacroDefs> */


/* <TypeDefs> */
typedef struct item {
    int data;
    float key;
} Item;

struct heap {
    int size;
    int nele;
    Item *items;
    int *map;
};
/* </TypeDefs> */


/*
Internal Method, swap the position of two Item object with pointer
 */
static void heap_swap(Heap *h, int a, int b);

/*
Internal Method, swap the position of two Item object with pointer
 */
static void item_swap(Item *a, Item *b);

/*
Internal Method, swap the map position of two Item object with pointer
 */
static void map_swap(int *a, int *b);

/*
Sift up element in heap with index [i]
 */
static void siftup(Heap *h, int i);

/*
Sift down element in heap with index [i]
 */
static void siftdown(Heap *h, int i);

/*
Return the index of min child of index [i],
-1 if no child is present (i.e. [i] is a leaf node)
 */
static int min_child(Heap *h, int i);



// /*Test Use Interface*/
// void print_heap(Heap *h) {
//     for (int i =0; i < h->nele; i++) {
//         printf("%d %d %f\n", h->items[i].data, h->map[h->items[i].data],
//                h->items[i].key);
//     }
//     printf("--------------------------------------------------------\n");
// }
// int
// main(int argc, char *argv[]) {
//     int i = 0;
//     Heap *h = new_heap(10);
//     for (i = 0; i < 10; i++) {
//         heap_insert(h, (float)((-i*134+i*i*178+37*i*i*i+67+9*i*i*i*i*i)%90), i);
//     }
//     print_heap(h);
//     // for (i=0; i < 10; i++) {
//     //     printf("Item key = %f, Item data = %d, Item map_position=%d\n",
//     //            heap_peek_key(h), heap_peek_min(h), h->map[heap_peek_min(h)]);
//     //     heap_remove_min(h);
//     // }
//     heap_change_key(h, heap_peek_min(h), 500);
//     print_heap(h);
//     heap_change_key(h, h->items[7].data, -1);
//     print_heap(h);
//     free_heap(h);
//     return 0;
// }


static void heap_swap(Heap *h, int a, int b) {
    item_swap(h->items+a, h->items+b);
    map_swap(h->map+((h->items+a)->data), h->map+((h->items+b)->data));
}


static void item_swap(Item *a, Item *b) {
    Item c = *a;
    *a = *b;
    *b = c;
}


static void map_swap(int *a, int *b) {
    int c = *a;
    *a = *b;
    *b = c;
}


Heap *new_heap(int max_size) {
    int i = 0;
    Heap* h = (Heap*)malloc(sizeof(Heap));
    assert(h!=NULL);

    /* Set initial value */
    h->size = max_size;
    h->nele = 0;
    h->items = (Item*)malloc(max_size*sizeof(Item));
    assert(h->items!=NULL);
    h->map = (int*)malloc(max_size*sizeof(int));
    assert(h->map!=NULL);
    for (i=0; i<h->size; i++) {
        h->map[i] = INVALID;
    }
    return h;
}

int heap_show_nele(Heap *h) {
    assert(h!=NULL);

    return h->nele;
}


int heap_show_size(Heap *h) {
    assert(h!=NULL);

    return h->size;
}


void heap_insert(Heap *h, float key, int data) {
    assert(h!=NULL);
    assert(h->nele < h->size);

    h->items[h->nele].data = data;
    h->items[h->nele].key = key;
    h->map[data] = h->nele;
    h->nele++;
    siftup(h, h->nele-1);
}


int heap_remove_min(Heap *h) {
    assert(h!=NULL);
    assert(h->nele >= 1);

    /* When heap is not empty get the data */
    int data = heap_peek_min(h);
    /* Swap the last item up and sort it into correct place */
    heap_swap(h, 0, h->nele-1);
    h->map[data] = INVALID;
    h->nele--;
    if (h->nele > 1) {
        siftdown(h, 0);
    }

    return data;
}


int heap_replace_min(Heap *h, float key, int data) {
    assert(h!=NULL);
    assert(h->nele >= 1);

    /* When heap is not empty get the data */
    int min_data = heap_peek_min(h);
    /* replace the first item */
    h->items[0].data = data;
    h->items[0].key = key;
    /* siftdown to correct position */
    siftdown(h, 0);
    return min_data;
}

void heap_change_key(Heap *h, int data, float new_key) {
    assert(data < h->size);
    int ind = h->map[data];
    assert(ind != -1);
    int old_key = (h->items+ind)->key;
    (h->items+ind)->key = new_key;
    if (new_key < old_key) {
        siftup(h, ind);
    } else {
        siftdown(h, ind);
    }
}


int heap_peek_min(Heap *h) {
    assert(h!=NULL);
    assert(h->nele >= 1);

    return h->items[0].data;
}


float heap_peek_key(Heap *h) {
    assert(h!=NULL);
    assert(h->nele >= 1);

    return h->items[0].key;
}


void free_heap(Heap *h) {
    assert(h!=NULL);

    free(h->items);
    free(h);
    free(h->map);
}




static void siftup(Heap *h, int i) {
    assert(h!=NULL);
    assert(i < h->nele);

    int parent = (i - 1) / 2;

    /* When [i] is not first element keep checking */
    /* Use while loop to increase efficiency */
    while (i > 0) {
        if (h->items[i].key < h->items[parent].key) {
            heap_swap(h, i, parent);
            i = parent;
            parent = (i - 1) / 2;
        } else {
            /* Break if sorted in place */
            break;
        }
    }
}

static void siftdown(Heap *h, int i) {
    assert(h!=NULL);
    assert(i < h->nele);

    /* If need to swap, swap with the minimum child */
    int child = min_child(h, i);

    /* When there are still child */
    while (child != -1) {
        if (h->items[child].key < h->items[i].key) {
            heap_swap(h, i, child);
            i = child;
            child = min_child(h, i);
        } else {
            /* Break if sorted in place */
            break;
        }
    }
}

static int min_child(Heap *h, int i) {
    /* Set default to the first child */
    int child = 2 * i + 1;

    /* Return -1 if [i] is leaf node */
    if (child >= h->nele) {
        return -1;
    }
    /* If second child exist and is less than the first, set second child */
    if (child+1 < h->nele && h->items[child+1].key < h->items[child].key){
        child++;
    }
    return child;
}