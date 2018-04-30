/*****************************************************************************
 *Author          Calvin Zhuoqun Huang ()
 *Licence         None
 *File Name       main_bbst.c
 *Space for Tab   YES
 *Tab size        4

 *Brief:
    Test main for bbst
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../poly_bbst.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
#define INVALID -1

#define NITEM 7
/* </MacroDefs> */


/* <TypeDefs> */
/* typedef */
/* </TypeDefs> */


/* <FunctionPrototypes> */
/* –––––––––––––––––––– */
/* </FunctionPrototypes> */

int cmp(void *a, void *b) {
    return *(int*)a - *(int*)b;

}

int
main(int argc, char *argv[]) {
    // int *keys = (int[13]){1,2,3,4,5,6,7,8,9,10,11,12,13};
    // float *data = (float[13]){1.1,2.2,3.3,4.4,5.5,6.6,7.7,8.8,9.9,
    //                             10.1,11.11,12.12,13.13};
    int *keys = (int[NITEM]){11, 6, 9, 4, 2, 7, 8};
    int *test_keys = (int[12]){0,1,2,3,4,5,6,7,8,9,10,11};
    float *data = (float[100]){1.1,2.2,3.3,4.4,5.5,6.6, 7.7, 8.8, 9.9, 10.1};
    Tree *t = new_tree(sizeof*keys, sizeof*data, cmp);
    for (int i = 0; i < NITEM; i++) {
        printf("=============== i is %d==========================================\n", i);
        tree_insert(t, keys+i, data+i);
    }
    print_tree(t);
    for (int i = 0; i < NITEM; i++) {
        printf("Trying to find %d\n", keys[i]);
        check_data(t, keys+i);
    }
    printf("test internal structure\n");
    for (int i = 0; i < 12; i++) {
        printf("Trying to find %d\n", test_keys[i]);
        check_data(t, test_keys+i);
    }
    return 0;
}



