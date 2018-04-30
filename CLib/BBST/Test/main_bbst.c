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

#define NITEM 34
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
    int *keys = (int[1000]){11, 6, 9, 4, 2, 7, 8, 0,1,2,3,4,5,6,7,8,9,10,11,
                                        50,31,22,3,54,5,66,7,87,99,80,91, 87,99,80,91};
    int *test_keys = (int[12]){0,1,2,3,4,5,6,7,8,9,10,11};
    float *data = (float[1000]){1.1,2.2,3.3,4.4,5.5,6.6, 7.7, 8.8, 9.9, 10.1, 11.11,
                                12.12,13.13, 14.14, 15.15, 16.16, 17.17, 18.18, 19.19,
                                20.20, 21.21,22.22,23.23,24.24,25.25, 26.26, 27.27, 28.28,
                                29.29, 30.30, 31.31, 32.32, 33.33, 34.34, 35.35};
    int tmp_key;
    float tmp_data;
    Tree *t = new_tree(sizeof*keys, sizeof*data, cmp);
    for (int i = 0; i < NITEM; i++) {
        printf("=============== i is %d==========================================\n", i);
        tree_insert(t, keys+i, data+i);
    }
    printf("total %d elements\n", tree_nele(t));
    printf("Retireve\n");
    retrieve_tree(t, NULL, NULL);
    while (retrieve_tree(t, &tmp_key, &tmp_data) != INVALID) {
        printf("key is %d data is %lf\n", tmp_key, tmp_data);
    }
    free_tree(t);
    return 0;
}



