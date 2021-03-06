/*
 * 
 * A proportion of codes in this implementation are adapted from book: 
 *         The Algorithm Design Manual by Steven Skiena
 * Reference Book <The Algorithm Design Manual> more detail
 *
 * Structure includes:
 *     Edgenode, Graph.
 * 
 * These function includes:
 *     initialize_graph, read_graph, insert_edge, initialize_search, dfs, bfs.
 *     
 *
 * 
 * No copyright received.
 */

/* Header guard */
#ifndef _GRAPH_H
#define _GRAPH_H

#define MAXV 1000 /* maximum number of vertices */

typedef struct graph_s Graph;

typedef struct search_s {
    bool processed[MAXV];   /* which vertices have been processed */
    bool discovered[MAXV];  /* which vertices have been found */
    int parent[MAXV];       /* discovery relation */
    int entry_time[MAXV];   /* entry time of dfs*/
    int exit_time[MAXV];    /* entry time of dfs*/
    int time;               /* record the current entry time */
} Search;

typedef struct edgenode_s {
    int y;                   /* adjacency info */
    int weight;              /* edge weight, if any */
    struct edgenode_s *next; /* next edge in list */
} Edgenode;

struct graph_s {
    Edgenode *edges[MAXV];   /* adjacency info */
    int degree[MAXV];        /* outdegree of each vertex */
    int nvertices;           /* number of vertices in graph */
    int nedges;              /* number of edges in graph */
    bool directed;           /* is the graph directed? */
    bool weighted;           /* is the graph weighted? */
    Search search;
};

typedef struct dist_s {
    int start;
    int *dist;
} Dist;



/*
 * initialize the graph
 */
Graph *new_graph(bool directed, bool weighted);

/*
 * Read the graph to struct
 * 
 * the first %d %d for number of vertices and edges
 * then each %d %d denote edges.
 */
Graph *read_graph(bool directed, bool weighted);

/*
 * Insert edge to graph
 */
void insert_edge(Graph *g, int x, int y, int w);

/*
 * Print the graph
 */
void print_graph(Graph *g);

/*
 * free edgenodes in the graph
 */
void free_graph(Graph *g);

/*
 * Initialize the search (Both bfs and dfs)
 */
void initialize_search(Graph *g);

/*
 * Breath First search (Need initialize search first)
 */
void bfs(Graph *g, int start);

/*
 * Depth First search (Need initialize search first)
 */
void dfs(Graph *g, int start);

/*
 * Whole graph depth first search
 */
void full_dfs(Graph *g);

/*
 * Process the edge of from node v-> node y
 */
void process_edge(Graph *g, int v, int y);

/*
 * Early process the vertex
 */
void process_vertex_early(Graph *g, int v);

/*
 * Late process the vertex
 */
void process_vertex_late(Graph *g, int v);

/*
 * dijkstra algorithm search over the whole graph starting from point start
 */
Dist *dijkstra_search(Graph *g, int start);

/*
 * dijkstra algorithm for finding single path from point start to end
 */
int dijkstra_path(Graph *g, int start, int end);

/*
 * Prim algorithm for finding one of possible minimum spanning tree
 * return a Graph containing only minimal spanning edges
 ** For weighted graph only
 * g is pointer to original graph
 * gprim is an UNINITIALIED Graph!!
 */
Graph *prim(Graph *g);


#endif


