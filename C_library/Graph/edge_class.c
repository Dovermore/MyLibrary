/*****************************************************************************
 *Author   : Calvin Zhuoqun Huang
 *Licence  : None
 *File Name: edge_class.c
 
 *Brief:
    Edge classification task in graph
 *It's not supposed to be shared without permission of author.
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "graph.h"

/* <MacroDefs> */
#define         TRUE 1
#define        FALSE 0
#define    TREE_EDGE 0
#define FORWARD_EDGE 1
#define    BACK_EDGE 2
#define   CROSS_EDGE 3
#define    LOOP_EDGE 4
#define INVALID_EDGE -1
/* </MacroDefs> */


/* <TypeDefs> */
/* typedef */
/* </TypeDefs> */

/* <FunctionPrototypes> */
int class_edge(Graph *g, int v, int y);
/* </FunctionPrototypes> */

/*
 * Process the edge of from node v-> node y
 */
void process_edge(Graph *g, int v, int y) {
    int class = -1;
    class = class_edge(g, v, y);
    printf("edge %d->%d is", v, y);
    if (class==TREE_EDGE) printf(" TREE_EDGE\n");
    if (class==FORWARD_EDGE) printf(" FORWARD_EDGE\n");
    if (class==BACK_EDGE) printf(" BACK_EDGE\n");
    if (class==CROSS_EDGE) printf(" CROSS_EDGE\n");
    if (class==LOOP_EDGE) printf(" LOOP_EDGE\n");
    return;
}

/*
 * Early process the vertex
 */
void process_vertex_early(Graph *g, int v) {
    printf("Enter vertex %d\n", v);
    return;
}

/*
 * Late process the vertex
 */
void process_vertex_late(Graph *g, int v) {
    printf("Exit vertex %d\n", v);
    return;
}

int class_edge(Graph *g, int v, int y) {
    if (!g->directed) {
        if (g->search.processed[y]) {
            printf("ERROR! Path from %d to %d where %d is processed\n",
                   v, y, v);
            return INVALID_EDGE;
        } else if (g->search.discovered[y]) {
            if (g->search.entry_time[v] == g->search.entry_time[y]) {
                return LOOP_EDGE;
            }
            return BACK_EDGE;
        } else {
            return TREE_EDGE;
        }
    } else {
        if (!g->search.discovered[y]) {
            return TREE_EDGE;
        } else if (!g->search.processed[y]) {
            if (g->search.entry_time[v] == g->search.entry_time[y]) {
                return LOOP_EDGE;
            }
            return BACK_EDGE;
        } else if (g->search.entry_time[v] < g->search.entry_time[y]) {
            return FORWARD_EDGE;
        } else {
            return CROSS_EDGE;
        }
    }
}