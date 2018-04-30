/*****************************************************************************
 *Author   : Calvin Huang
 *Licence  : None
 *File Name: Heap.h
 
 *This program is created for the purpose of:
    Preparation of Heap sort. And implements normal heap operations.
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "heap&heap_sort.h"

typedef void *heap_t;


//********************************************************************

/*
typedef int data_t;

int cmpint(void *a, void *b) {
    data_t *a1 = (data_t*)a,
        *b1 = (data_t*)b;
    return *b1 - *a1;
}


void swapint(void *a, void *b) {
  data_t c; // C99, use malloc otherwise
  // char serves as the type for "generic" byte arrays
  memcpy(&c,  b, sizeof(data_t));
  memcpy( b,  a, sizeof(data_t));
  memcpy( a, &c, sizeof(data_t));
}*/


//********************************************************************


/*
int
main(int argc, char *argv[]) {
    srand(clock());
    // srand(2);
    int i = 0,
        // thresh = 4,
        thresh = rand() % 15,
        ary[thresh];
    for (i = 0; i < thresh; i++) {
        printf("|%4d", i);
    }
    printf("\n");
    for (i = 0; i < thresh; i++) {
        printf("-----");
    }
    printf("\n");
    for (i = 0; i < thresh; i++) {
        ary[i] = rand() % 99;
        printf("|%4d", ary[i]);
    }
    printf("\n");
    for (i = 0; i < thresh; i++) {
        printf("|%4d", ary[i]);
    }
    printf("\n");
    // build_heap(ary, thresh, sizeof(int), cmpint, swapint);
    heap_sort(ary, thresh, sizeof(int), cmpint, swapint);
    printf("After Sort:\n");
    for (i = 0; i < thresh; i++) {
        printf("|%4d", ary[i]);
    }
    printf("\n");
    return 0;
}
*/





void build_heap(void *base, size_t ntimes, size_t size,
                        int (*cmp)(void*, void*), void (*swap)(void*, void*)) {
    int i = ntimes / 2 - 1;
    for (i = ntimes / 2 - 1; i >= 0; i--) {
        // printf("build %d\n", i);
        sift_down((char*)base, ntimes, size, i, cmp, swap);
    }
}


void sift_down(char *base, size_t ntimes, size_t size, int parent,
                        int (*cmp)(void*, void*), void (*swap)(void*, void*)) {
    int child = 2 * parent + 1;
    if (child < ntimes) {
        if (child + 1 < ntimes && cmp((void*)base + child * size,
                                      (void*)base + size * (child + 1)) > 0) {
            child++;
        }
        if (cmp((void*)base + parent * size, (void*)base + child * size) > 0) {
            swap((void*)base + parent * size, (void*)base + child * size);
            sift_down(base, ntimes, size, child, cmp, swap);
        }
    }
}



void heap_sort(void *base, size_t ntimes, size_t size,
                        int (*cmp)(void*, void*), void (*swap)(void*, void*)) {
    int i   = 0;
    build_heap(base, ntimes, size, cmp, swap);
    for (i = 0; i < ntimes; i++) {
        swap(base, base + (ntimes - 1 - i) * size);
        sift_down((char*)base, ntimes - i - 1, size, 0, cmp, swap);
    }
}