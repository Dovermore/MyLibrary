#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

/*
 * Process the edge of from node v-> node y
 */
void process_edge(Graph *g, int v, int y) {
    printf("    Processing edge %d->%d\n", v, y);
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