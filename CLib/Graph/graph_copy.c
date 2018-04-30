/*
 * 
 * A proportion of codes in this implementation are adapted from book: 
 *         The Algorithm Design Manual by Steven Skiena
 * Reference Book <The Algorithm Design Manual> more detail
 *
 * These function includes:
 *     initialize_graph, read_graph, insert_edge, initialize_search, dfs, bfs
 * 
 * And the implementation of linked_list, queue, stack modules are adapted
 * from Matt Farrugia's implementation for COMP20007.
 *
 * Detail reference:
 *            created for COMP20007 Design of Algorithms 2017
 *            by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 * 
 * No copyright received.
 */

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../LinkedList/list.h"
#include "../LinkedList/queue.h"
#include "../LinkedList/stack.h"
#include "../Heap/heap.h"
#include "graph.h"



/* <MacroDefs> */
#define TRUE 1
#define FALSE 0
/* </MacroDefs> */


void initialize_graph(Graph *g, bool directed, bool weighted) {
    int i; /* counter */
    g->nvertices = 0;
    g->nedges = 0;
    g->directed = directed;
    g->weighted = weighted;
    for (i=0; i<MAXV; i++) g->degree[i] = 0;
    for (i=0; i<MAXV; i++) g->edges[i] = NULL;
}


void read_graph(Graph *g, bool directed, bool weighted) {
    int i; /* counter */
    int m; /* number of edges */
    int x, y, w=0; /* vertices in edge (x,y) and weight w */
    initialize_graph(g, directed, weighted);
    scanf("%d %d",&(g->nvertices),&m);
    if (weighted) {
        for (i=0; i<m; i++) {
            scanf("%d %d %d",&x,&y,&w);
            insert_edge(g,x,y,w);
        }
    } else {
        for (i=0; i<m; i++) {
            scanf("%d %d",&x,&y);
            insert_edge(g,x,y,w);
        }
    }
}


void insert_edge(Graph *g, int x, int y, int w) {
    Edgenode *p=(Edgenode*)malloc(sizeof(Edgenode)); /* temporary pointer */
    assert(p!=NULL);

    p->y = y;
    if (g->weighted) {
        p->weight = w;
    } else {
        p->weight=INT_MAX;
    }
    p->next = g->edges[x];
    g->edges[x] = p; /* insert at head of list */
    g->degree[x]++;


    if (g->directed == FALSE && x != y){
        p=(Edgenode*)malloc(sizeof(Edgenode));      /* temporary pointer */
        p->y = x;
        if (g->weighted) {
            p->weight = w;
        } else {
            p->weight=INT_MAX;
        }
        p->next = g->edges[y];
        g->edges[y]=p;
        g->degree[y]++;
    }
    else {
        g->nedges++;
    }
}


void print_graph(Graph *g) {
    assert(g != NULL);
    int i;       /* counter */
    Edgenode *p; /* temporary pointer */
    for (i=0; i<g->nvertices; i++) {
        printf("%d: ",i);
        p = g->edges[i];
        while (p != NULL) {
            printf(" %d",p->y);
            if (g->weighted) {
                printf("(%d)", p->weight);
            }
            p = p->next;
        }
        printf("\n");
    }
}

static void recursive_free_edgenodes(Edgenode *p) {
    if (p!=NULL) {
        recursive_free_edgenodes(p->next);
        free(p);
    }
}


void free_graph(Graph *g) {
    int i = 0;
    Edgenode *p=NULL;
    for (i=0; i<g->nvertices; i++) {
        p = g->edges[i];
        recursive_free_edgenodes(p);
    }
}


void initialize_search(Graph *g) {
    int i; /* counter */
    g->search.time = 0;
    for (i=1; i<=g->nvertices; i++) {
        g->search.processed[i] = g->search.discovered[i] = FALSE;
        g->search.parent[i] = -1;
        g->search.entry_time[i] = -1;
        g->search.exit_time[i] = -1;
    }
}

void bfs(Graph *g, int start) {
    Queue *q=NULL; /* queue of vertices to visit */
    int v;         /* current vertex */
    int y;         /* successor vertex */
    Edgenode *p;   /* temporary pointer */
    init_queue(&q);
    enqueue(q,start);
    g->search.discovered[start] = TRUE;
    while (empty_queue(q) == FALSE) {
        v = dequeue(q);
        process_vertex_early(g, v);
        g->search.processed[v] = TRUE;
        p = g->edges[v];
        while (p != NULL) {
            y = p->y;
            // printf("y is %d\n", y);
            if ((g->search.processed[y] == FALSE) || g->directed)
                process_edge(g, v,y);
            if (g->search.discovered[y] == FALSE) {
                // printf("enqueue %d\n", y);
                enqueue(q,y);
                g->search.discovered[y] = TRUE;
                g->search.parent[y] = v;
            }
            p = p->next;
        }
        process_vertex_late(g, v);
    }
}

void dfs(Graph *g, int v) {
    int y=-1;
    Edgenode *p;
    g->search.discovered[v] = TRUE;
    g->search.entry_time[v] = g->search.time;
    g->search.time++;
    process_vertex_early(g, v);
    g->search.discovered[v] = TRUE;

    p = g->edges[v];
    while (p!=NULL) {
        y = p->y;
        // printf("y is %d\n", y);
        if (g->search.discovered[y] == FALSE){
            g->search.parent[y] = v;
            process_edge(g, v, y);
            dfs(g, y);
        } else if (g->directed||(!g->search.processed[y] 
                                    && g->search.parent[v] != y)) {
            // printf("edge of discovered node %d, of %d->%d\n", y, v, y);
            process_edge(g, v, y);
        }
        p = p->next;
    }
    process_vertex_late(g, v);
    g->search.exit_time[v] = g->search.time;
    g->search.time++;
    g->search.processed[v] = TRUE;
}

void full_dfs(Graph *g) {
    int i;
    for (i = 0; i < g->nvertices; i++) {
        if (!g->search.discovered[i]) {
            dfs(g, i);
        }
    }
}


Dist *dijkstra_search(Graph *g, int start) {
    assert(g->weighted!=FALSE);
    Heap *h = new_heap(g->nvertices);       /* Heap for find min distance */
    Edgenode *p = NULL;

    Dist *fd = (Dist*)malloc(sizeof*fd);
    fd->start = start;
    fd->dist = (int*)malloc(g->nvertices*sizeof*(fd->dist));

    int i = 0,
        dist[g->nvertices],
        v = -1,
        y = -1,
        w = -1;

    for (i = 0; i < g->nvertices; i++) {
        dist[i] = INT_MAX;
    }

    dist[start] = 0;
    g->search.discovered[start] = TRUE;
    heap_insert(h, (float)dist[start], start);

    while (heap_show_nele(h) > 0) {
        v=heap_remove_min(h);
        // printf("v is %d\n", v);
        g->search.processed[v] = TRUE;
        p=g->edges[v];
        while (p!=NULL) {
            y = p->y;
            // printf("y is %d\n", y);
            if (g->search.processed[y]==TRUE) {
                p = p->next;
                continue;
            }
            w = p->weight;
            if (g->search.discovered[y]==FALSE) {   /* If hasn't found node */
                // printf("Undiscovered %d\n", y);
                g->search.discovered[y]=TRUE;
                dist[y] = (dist[v]+w);
                heap_insert(h, (float)dist[y], y);
                g->search.parent[y] = v;
            } else if (dist[y] > dist[v]+w) {       /* Found a shorter path */
                // printf("Found shorter path %d %d\n", dist[y], dist[v]+w);
                dist[y] = dist[v]+w;
                heap_change_key(h, y, (float)dist[y]);
                g->search.parent[y] = v;
            }
            // printf("next p\n");
            p = p->next;
        }
    }

    for (i=0; i<g->nvertices; i++) {
        fd->dist[i] = dist[i];
    }

    return fd;
}


int dijkstra_path(Graph *g, int start, int end) {
    assert(g->weighted!=FALSE);
    Heap *h = new_heap(g->nvertices);       /* Heap for find min distance */
    Edgenode *p = NULL;

    int i = 0,
        dist[g->nvertices],
        v = -1,
        y = -1,
        w = -1;

    for (i = 0; i < g->nvertices; i++) {
        dist[i] = INT_MAX;
    }

    dist[start] = 0;
    g->search.discovered[start] = TRUE;
    heap_insert(h, (float)dist[start], start);

    while (heap_show_nele(h) > 0) {
        v=heap_remove_min(h);
        if (v == end) {             /* Found the shortest path */
            return dist[v];
        }

        // printf("v is %d\n", v);
        g->search.processed[v] = TRUE;
        p=g->edges[v];
        while (p!=NULL) {
            y = p->y;
            // printf("y is %d\n", y);
            if (g->search.processed[y]==TRUE) {
                p = p->next;
                continue;
            }
            w = p->weight;
            if (g->search.discovered[y]==FALSE) {   /* If hasn't found node */
                // printf("Undiscovered %d\n", y);
                g->search.discovered[y]=TRUE;
                dist[y] = (dist[v]+w);
                heap_insert(h, (float)dist[y], y);
                g->search.parent[y] = v;
            } else if (dist[y] > dist[v]+w) {       /* Found a shorter path */
                // printf("Found shorter path %d %d\n", dist[y], dist[v]+w);
                dist[y] = dist[v]+w;
                heap_change_key(h, y, (float)dist[y]);
                g->search.parent[y] = v;
            }
            // printf("next p\n");
            p = p->next;
        }
    }
    printf("There is no path from point %d to %d\n", start, end);
    return INT_MAX;
}


void prim(Graph *g, Graph *gprim) {
    assert(g->weighted);

    Heap *h = new_heap(g->nedges);
    Edgenode *p;

    int v    = 0,
        y    = 0,
        w    = 0,
        i    = 0,
        n    = 0,
        ind  = 0,
        cont = 1,
        starts[g->nedges],
        ends  [g->nedges];

    initialize_graph(gprim, g->directed, g->weighted);
    gprim->nvertices = g->nvertices;

    for (i = 0; i < g->nvertices; i++) {
        if (g->search.processed[i]) continue;

        v = i;
        g->search.discovered[v] = TRUE;
        while (cont) {
            p = g->edges[v];
            while (p != NULL) {
                y = p->y;
                w = p->weight;
                g->search.discovered[y] = TRUE;
                if (!g->search.processed[y] || !g->search.processed[v]) {
                    starts[n] = v;
                    ends  [n] = y;
                    heap_insert(h, (float)w, n);
                    n++;
                }
                p = p->next;
            }

            cont = 0;
            if (!heap_show_nele(h)) break;
            while (heap_show_nele(h)) {
                w   = heap_peek_key(h);
                ind = heap_remove_min(h);
                if (!g->search.processed[starts[ind]] || 
                                    !g->search.processed[ends[ind]]) {
                    cont = 1;
                    insert_edge(gprim, starts[ind], ends[ind], w);
                    g->search.processed[starts[ind]] = TRUE;
                    g->search.processed[ends[ind]] = TRUE;
                    v = ends[ind];
                    break;
                }
            }
        }
    }
}
