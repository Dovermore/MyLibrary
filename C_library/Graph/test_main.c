/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: test_main.c
 
 *Brief:
    Enter The brief of program test_main.c
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "test.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
/* </MacroDefs> */


/* <TypeDefs> */
/* typedef */
/* </TypeDefs> */



int
main(int argc, char *argv[]) {
    int i = NULL;
    wrap_func();
    if (i == 0) {
        printf("0\n");
    } else if (i == NULL) {
        printf("NULL\n");
    }
    printf("%d\n", i);
    return 0;
}