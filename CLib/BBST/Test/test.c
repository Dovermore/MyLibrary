/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: test.c
 
 *Brief:
    For testing implementations
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
#define INVALID -1
/* </MacroDefs> */

int
main(int argc, char *argv[]) {
    printf("%d\n", 1);
    printf("%d\n", !1);
    printf("%d\n", 0);
    printf("%d\n", !0);
    return 0;
}