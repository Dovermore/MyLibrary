/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: main_hash.c
 
 *Brief:
    Testing file for inthash library
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "inthash.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
#define SHOW 1
/* </MacroDefs> */


/* <TypeDefs> */
/* typedef */
/* </TypeDefs> */


/* <FunctionPrototypes> */
/* –––––––––––––––––––– */
/* </FunctionPrototypes> */



int
main(int argc, char *argv[]) {
    srand(clock());
    int nele = 4, key, data;
    int num = 0;
    int numbers[nele+5];

    HashTable *h = new_hash_table(nele);
    for (int i = 0; i < nele + 500; i++) {
        key = rand()%1000;
        data = rand()%10;
        if (rand()%2 && num < nele+5) {
            numbers[num] = key + rand()%3;
            num++;
        }
        // printf("%d %d\n", key, data);
        hash_insert(h, key, data);
    }

    print_hash_table(h, SHOW);


    printf("Testing doubling table: \n");

    h = double_and_rehash(h);

    printf("Testing doubling table: \n");

    h = double_and_rehash(h);

    printf("Testing doubling table: \n");

    h = double_and_rehash(h);

    printf("Testing doubling table: \n");

    h = double_and_rehash(h);

    printf("Testing doubling table: \n");

    h = double_and_rehash(h);

    printf("Testing doubling table: \n");

    h = double_and_rehash(h);

    printf("Testing doubling table: \n");

    h = double_and_rehash(h);

    print_hash_table(h, SHOW);
    free_hash_table(h);
    return 0;
}