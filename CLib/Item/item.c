/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: item.c
 
 *Brief:
    For general item class implementation
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "item.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
/* </MacroDefs> */

void item_swap(Item *a, Item *b) {
    Item c = *a;
    *a = *b;
    *b = c;
}