/**
 * Graph Adjancency Matric implementation.
 * Copyright (C) 2022 Lazar Razvan.
 */

#include <stdbool.h>

/****************************** DATA STRUCTURE ********************************/

#define GRAPH_MAX_NODES				50

typedef struct graph {

	unsigned char	nodes;
	char			**mat;
	unsigned char	*visited;

} graph_t;

/******************************** PUBLIC API **********************************/

/**
 * Create/Destroy.
 */
graph_t *graph_create(unsigned char);
void graph_destroy(graph_t *);

/**
 * Edges.
 */
int graph_add_directed_edge(graph_t *, unsigned char, unsigned char);
int graph_add_undirected_edge(graph_t *, unsigned char, unsigned char);
int graph_add_directed_wedge(graph_t *, unsigned char, unsigned char, char);
int graph_add_undirected_wedge(graph_t *, unsigned char, unsigned char, char);

/**
 * BFS.
 */
int graph_bfs(graph_t *, unsigned char);
int graph_node_level(graph_t *, unsigned char , char *);

/**
 * DFS.
 */
int graph_dfs(graph_t *, unsigned char);
int graph_detect_cycle(graph_t *, bool *);
int graph_max_length(graph_t *, int *);

/**
 * Bellman-Ford.
 */
int graph_bellman_ford(graph_t *, unsigned char, bool *);
