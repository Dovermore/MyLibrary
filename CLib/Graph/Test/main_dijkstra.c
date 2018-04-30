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
#include "../graph.h"

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
        wei = 0,
        i   = 0,
        start=0,
        end = 7;
    Dist *di=NULL;
    if (argc > 1) {
        dir = atoi(argv[1]);
        if (argc > 2 ) {
            wei = atoi(argv[2]);
        }
    }

    read_graph(&g, dir, wei);
    printf("dijkstra_search\n");
    initialize_search(&g);
    di = dijkstra_search(&g, start);
    for (i=0; i<g.nvertices; i++) {
        printf("To %d dist %d\n", i, di->dist[i]);
    }
    printf("dijkstra_path\n");
    initialize_search(&g);
    i = dijkstra_path(&g, start, end);
    printf("from point %d to point %d intotal took %d time\n", start, end, i);
    print_graph(&g);
    free_graph(&g);
    return 0;
}