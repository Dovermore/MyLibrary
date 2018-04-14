/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: main.c
 
 *Brief:
    File for testing implementation of graph and etc...
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "graph.h"
#include "stack.h"
#include "queue.h"
#include "heap.h"

/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
/* </MacroDefs> */


/* <TypeDefs> */
/* typedef */
/* </TypeDefs> */



int
main(int argc, char *argv[]) {
    Graph g;
    int dir = 0,
        wei = 0;
    if (argc > 1) {
        dir = atoi(argv[1]);
        if (argc > 2 ) {
            wei = atoi(argv[2]);
        }
    }

    read_graph(&g, dir, wei);
    printf("dfs\n");
    initialize_search(&g);
    dfs(&g, 0);
    return 0;
}