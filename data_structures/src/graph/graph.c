/**
 * Graph Adjancency Matric implementation.
 * Copyright (C) 2022 Lazar Razvan.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "queue/queue.h"
#include "graph/graph.h"

/******************************** PUBLIC API **********************************/

/****************************** CREATE/DESTROY ********************************/

/**
 * Create a graph with a given numbers of nodes.
 *
 * @nodes	: Graph number of nodes.
 *
 * Return graph on success and NULL otherwise.
 */
graph_t *graph_create(unsigned char nodes)
{
	graph_t *graph = NULL;

	/* Limit validation */
	if (nodes > GRAPH_MAX_NODES)
		return NULL;

	/* Graph create */
	graph = malloc(sizeof(graph_t));
	if (!graph)
		return NULL;


	graph->nodes = nodes;

	// Visited nodes space
	graph->visited = calloc(nodes, sizeof(unsigned char));
	if (!graph->visited) {
		free(graph);
		return NULL;
	}

	// Adjancency matric space
	graph->mat = calloc(nodes, sizeof(char *));
	if (!graph->mat) {
		free(graph->visited);
		free(graph);
		return NULL;
	}

	for (int i = 0; i < graph->nodes; i++) {
		graph->mat[i] = calloc(nodes, sizeof(char));
		if (!graph->mat[i]) {
			for (int j = 0; j < i; j++)
				free(graph->mat[j]);

			free(graph->mat);
			free(graph->visited);
			free(graph);
			return NULL;
		}
	}

	return graph;
}

/**
 * Destroy (free space) for a graph.
 */
void graph_destroy(graph_t *graph)
{
	if (!graph)
		return;

	for (int i = 0; i < graph->nodes; i++)
		free(graph->mat[i]);

	free(graph->visited);
	free(graph->mat);
	free(graph);
}

/*********************************** EDGES ************************************/

/**
 * Add a directed edge between two nodes.
 *
 * @graph	: Graph data structure.
 * @s		: Source edge node.
 * @d		: Destination edge node.
 *
 * Return 0 on success and <0 otherwise.
 */
int graph_add_directed_edge(graph_t *graph, unsigned char s, unsigned char d)
{
	return graph_add_directed_wedge(graph, s, d, 1);
}

/**
 * Add an undirected edge between two nodes.
 *
 * @graph	: Graph data structure.
 * @s		: Source edge node.
 * @d		: Destination edge node.
 *
 * Return 0 on success and <0 otherwise.
 */
int graph_add_undirected_edge(graph_t *graph, unsigned char s, unsigned char d)
{
	return graph_add_undirected_wedge(graph, s, d, 1);
}

/**
 * Add a weighted directed edge between two nodes.
 *
 * @graph	: Graph data structure.
 * @s		: Source edge node.
 * @d		: Destination edge node.
 * @w		: Edge weight.
 *
 * Return 0 on success and <0 otherwise.
 */
int graph_add_directed_wedge(graph_t *graph, unsigned char s, unsigned char d,
							char w)
{
	if (s >= graph->nodes || d >= graph->nodes)
		return -1;

	graph->mat[s][d] = w;

	return 0;
}

/**
 * Add a weighted undirected edge between two nodes.
 *
 * @graph	: Graph data structure.
 * @s		: Source edge node.
 * @d		: Destination edge node.
 * @w		: Edge weight.
 *
 * Return 0 on success and <0 otherwise.
 */
int graph_add_undirected_wedge(graph_t *graph, unsigned char s, unsigned char d,
								char w)
{
	if (s >= graph->nodes || d >= graph->nodes)
		return -1;

	graph->mat[s][d] = w;
	graph->mat[d][s] = w;

	return 0;
}

/************************************ BFS *************************************/

/**
 * Graph Breadth First Search Traversal.
 *
 * @graph	: Graph data structure.
 * @s		: Traversal source node.
 *
 * Note that extra array is used ("nodes") to be able to add nodes addresses
 * to queue.
 *
 * Return 0 on success and <0 on error.
 */
int graph_bfs(graph_t *graph, unsigned char s)
{
	int rv = 0;
	void *elem;
	queue_t *queue = NULL;
	unsigned char curr, *nodes;

	/* Validations */
	if (!graph || s >= graph->nodes) {
		rv = -1; goto error;
	}

	/****************************************************************
	 * Allocate memory.
	 ***************************************************************/
	// nodes array (for queue)
	nodes = malloc(graph->nodes * sizeof(unsigned char));
	if (!nodes) {
		rv = -2; goto error;
	}
	for (int i = 0; i < graph->nodes; i++)
		nodes[i] = i;

	// reset visited array
	memset(graph->visited, 0, graph->nodes * sizeof(unsigned char));

	// queue data structure
	queue = queue_create(graph->nodes);
	if (!queue) {
		rv = -3; goto error_free_nodes;
	}

	/****************************************************************
	 * Introduce source node in queue and mark as visited.
	 ***************************************************************/
	graph->visited[s] = 1;
	if (queue_enqueue(queue, &nodes[s])) {
		rv = -4; goto error_free_queue;
	}

	/****************************************************************
	 * While queue is not empty, dequeue first, iterate in all unvisited
	 * neighbors, enqueue and mark as visited.
	 ***************************************************************/
	while (!queue_is_empty(queue)) {
		// dequeue
		elem = queue_dequeue(queue);
		if (!elem) {
			rv = -5; goto error_free_queue;
		}

		// print
		curr = *((unsigned char *)elem);
		printf("Node %d\n", curr);

		// iterate in unvisited neighbors
		for (int i = 0; i < graph->nodes; i++) {
			if (graph->mat[curr][i] && !graph->visited[i]) {
				graph->visited[i] = 1;
				if (queue_enqueue(queue, &nodes[i])) {
					rv = -6; goto error_free_queue;
				}
			}
		}
	}

error_free_queue:
	queue_destroy(queue);

error_free_nodes:
	free(nodes);

error:
	if (rv)
		printf("[%s] error %d\n", __func__, rv);

	return rv;
}

/**
 * Find the level of a given node in an undirected graph.
 *
 * @graph	: Graph data structure.
 * @s		: Node to find level for.
 * @level	: Node level (filled in on success)
 *
 * Given an undirected graph with V vertices and E edges and a node X. The task
 * is to find the level of node X in the undirected graph. If X does not exist
 * in the graph then return -1.
 *
 * Return 0 on success and <0 on error.
 */
int graph_node_level(graph_t *graph, unsigned char s, char *level)
{
	int rv = 0;
	void *elem;
	unsigned int q_size;
	queue_t *queue = NULL;
	unsigned char curr, *nodes;

	/* Validations */
	if (!graph) {
		rv = -1; goto error;
	}

	/****************************************************************
	 * Allocate memory.
	 ***************************************************************/
	// nodes array (for queue)
	nodes = malloc(graph->nodes * sizeof(unsigned char));
	if (!nodes) {
		rv = -2; goto error;
	}
	for (int i = 0; i < graph->nodes; i++)
		nodes[i] = i;

	// reset visited array
	memset(graph->visited, 0, graph->nodes * sizeof(unsigned char));

	// queue data structure
	queue = queue_create(graph->nodes);
	if (!queue) {
		rv = -3; goto error_free_nodes;
	}

	/**
	 * Start from node 0. Introduce node in queue and mark as visited and set
	 * level to 0.
	 */
	*level = 0;
	graph->visited[0] = 1;
	if (queue_enqueue(queue, &nodes[0])) {
		rv = -4; goto error_free_queue;
	}

	/**
	 * While queue is not empty, dequeue all elements in queue. If node is found
	 * stop the algorithm. Otherwise, insert all unvisited nodes in queue, mark
	 * as visited and increase the level.
	 */
	while (!queue_is_empty(queue)) {
		q_size = queue_size(queue);
		while (q_size--) {
			// dequeue
			elem = queue_dequeue(queue);
			if (!elem) {
				rv = -5; goto error_free_queue;
			}

			curr = *((unsigned char *)elem);
			if (curr == s) {
				rv = 0; goto error_free_queue;	// stop algorithm
			}

			// iterate in unvisited neighbors
			for (int i = 0; i < graph->nodes; i++) {
				if (graph->mat[curr][i] && !graph->visited[i]) {
					graph->visited[i] = 1;
					if (queue_enqueue(queue, &nodes[i])) {
						rv = -6; goto error_free_queue;
					}
				}
			}
		}
		(*level)++;
	}

	*level = -1;	// node not found

error_free_queue:
	queue_destroy(queue);

error_free_nodes:
	free(nodes);

error:
	if (rv)
		printf("[%s] error %d\n", __func__, rv);

	return rv;
}

/************************************ DFS *************************************/

static void __graph_dfs(graph_t *graph, unsigned char s)
{
	printf("Node %d\n", s);

	graph->visited[s] = 1;

	for (int i = 0; i < graph->nodes; i++) {
		if (graph->mat[s][i] && !graph->visited[i])
			__graph_dfs(graph, i);
	}
}

/**
 * Graph Depth First Search Traversal.
 *
 * @graph	: Graph data structure.
 * @s		: Traversal source node.
 *
 * Return 0 on success and <0 on error.
 */
int graph_dfs(graph_t *graph, unsigned char s)
{

	/****************************************************************
	 * Validation.
	 ***************************************************************/
	if (!graph || s >= graph->nodes)
		return -1;

	/****************************************************************
	 * Reset visited array.
	 ***************************************************************/
	memset(graph->visited, 0, graph->nodes * sizeof(unsigned char));

	/****************************************************************
	 * DFS.
	 ***************************************************************/
	__graph_dfs(graph, s);

	return 0;
}

static bool __graph_detect_cycle(graph_t *graph, unsigned char s, char p)
{
	graph->visited[s] = 1;

	for (int i = 0; i < graph->nodes; i++) {
		if (!graph->mat[s][i])
			continue;

		if (!graph->visited[i]) {
			if (__graph_detect_cycle(graph, i, s))
				return true;
		} else if (i != p) {
			return true;
		}
	}

	return false;
}

/**
 * Detect if a cycle exists in an undirected graph using dfs.
 *
 * @graph	: Graph data structure.
 *
 * Return 0 on success and <0 otherwise.
 */
int graph_detect_cycle(graph_t *graph, bool *has_cycle)
{
	/****************************************************************
	 * Validation.
	 ***************************************************************/
	if (!graph)
		return -1;

	/****************************************************************
	 * Reset visited array.
	 ***************************************************************/
	memset(graph->visited, 0, graph->nodes * sizeof(unsigned char));

	/****************************************************************
	 * Start recursion for each unvisited node.
	 ***************************************************************/
	for (int i = 0; i < graph->nodes; i++) {
		if (graph->visited[i])
			continue;

		if (__graph_detect_cycle(graph, i, -1)) {
			*has_cycle = true;
			return 0;
		}
	}

	*has_cycle = false;

	return 0;
}

static void __graph_max_length(graph_t *graph, unsigned char s, int curr_len,
								int *max_len)
{
	graph->visited[s] = 1;

	if (curr_len > *max_len)
		*max_len = curr_len;

	for (int i = 0; i < graph->nodes; i++) {
		if (graph->mat[s][i] && !graph->visited[i])
			__graph_max_length(graph, i, curr_len + graph->mat[s][i], max_len);
	}
}

/**
 * Get max length (u; v) in a connected undirected graph with no cycles.
 *
 * @graph	: Graph data strucutre.
 * @max_len	: Max length between two nodes (filled in on success).
 *
 * Return 0 on success and <0 otherwise.
 */
int graph_max_length(graph_t *graph, int *max_len)
{
	/****************************************************************
	 * Validation.
	 ***************************************************************/
	if (!graph)
		return -1;

	*max_len = -1;

	/****************************************************************
	 * Start recursion for each node and reset visited array.
	 ***************************************************************/
	for (int i = 0; i < graph->nodes; i++) {
		memset(graph->visited, 0, graph->nodes * sizeof(unsigned char));

		__graph_max_length(graph, i, 0, max_len);
	}

	return 0;
}

static void __graph_nlength_cycles(graph_t *graph, unsigned char src,
									unsigned char curr, int len, int *cycles_no)
{
	/****************************************************************
	 * Mark current node as visited in dfs.
	 ***************************************************************/
	graph->visited[curr] = 1;

	/****************************************************************
	 * Path of "len" length found.
	 *
	 * Check if there is an edge to source node and count the cycle.
	 * Note that node is marked as unvisited to be able to be visited
	 * again.
	 ***************************************************************/
	if (len == 0) {
		graph->visited[curr] = 0;

		if (graph->mat[curr][src])
			(*cycles_no)++;

		return;
	}



	/****************************************************************
	 * Perform dfs and decrease length for next recursion step. When
	 * returning from recursion, we mark the node as unvisited to be
	 * able to found another path.
	 ***************************************************************/
	for (int i = 0; i < graph->nodes; i++) {
		if (!graph->visited[i] && graph->mat[curr][i]) {
			__graph_nlength_cycles(graph, src, i, len - 1, cycles_no);
		}
	}

	graph->visited[curr] = 0;
}

/**
 * Given an undirected and connected graph and a number n, count total number
 * of cycles of length n in the graph. A cycle of length n simply means that
 * the cycle contains n vertices and n edges.
 *
 * graph	: Graph data structure.
 * n		: Length of a cycle.
 *
 * Return the number of cycles of n length found in the graph.
 */
int graph_nlength_cycles(graph_t *graph, int n)
{
	int cycles_no = 0;

	/****************************************************************
	 * Reset visited array.
	 ***************************************************************/
	memset(graph->visited, 0, graph->nodes * sizeof(unsigned char));

	/****************************************************************
	 * We need to compute cycles of length n. This can be reduced to
	 * finding paths of length (n-1) where destination node has an
	 * edge to source node.
	 ***************************************************************/
	for (int i = 0; i < graph->nodes - (n - 1); i++) {
		__graph_nlength_cycles(graph, i, i, n-1, &cycles_no);

		graph->visited[i] = true;
	}

	/****************************************************************
	 * Each cycle is counted twice.
	 ***************************************************************/
	return cycles_no / 2;
}

/******************************** BELLMAN FORD ********************************/
/**
 * Bellman Ford Algorithm.
 *
 * Compute shortest paths from a single source node to all other nodes in a
 * weighted directed graph. The difference Dijkistra algorithm is that edges
 * weight may be negative.
 *
 * Note that Bellman Ford algorithm doesn't work if a negative cycle is found
 * in graph.
 *
 * WARNING:
 * Implementation will not work for edges with weight 0 since adjancency matrix
 * is used and mat[i][j] = 0 is considered as no edge between i and j.
 *
 * @graph			: Graph data structure.
 * @s				: Algorithm source node.
 * @has_neg_cycle	: Flag set if negative cycle is found.
 *
 * Return 0 on success and <0 otheriwse.
 */
int graph_bellman_ford(graph_t *graph, unsigned char s, bool *has_neg_cycle)
{
	bool updated;
	int *dist = NULL;

	/****************************************************************
	 * Validation.
	 ***************************************************************/
	if (!graph || s >= graph->nodes)
		return -1;

	/****************************************************************
	 * Alloc space for distances array and initialize it with
	 * infinte values (INT_MAX) excepting source itself.
	 ***************************************************************/
	dist = malloc(graph->nodes * sizeof(int));
	if (!dist)
		return -2;

	for (int i = 0; i < graph->nodes; i++)
		dist[i] = INT_MAX;
	dist[s] = 0;

	/****************************************************************
	 * Algorithm relaxation step.
	 *
	 * Run the relaxation |N| - 1 times (N - number of nodes) for each
	 * edge because there can be maximum (N-1) edges in a simple path.
	 *
	 * For every edge (u, v):
	 *
	 * if (dist[v] > dist[u] + weight(u,v))
	 *     dist[v] = dist[u] + weight(u,v)
	 ***************************************************************/
	for (int relax = 0; relax < graph->nodes - 1; relax++) {
		updated = false;

		for (int src = 0; src < graph->nodes; src++) {
			for (int dst = 0; dst < graph->nodes; dst++) {
				if (!graph->mat[src][dst])
					continue;

				if (dist[src] == INT_MAX)
					continue;

				if (dist[dst] > dist[src] + graph->mat[src][dst]) {
					updated = true;
					dist[dst] = dist[src] + graph->mat[src][dst];
				}
			}
		}

		if (!updated)
			break;
	}

	/****************************************************************
	 * Algorithm negative cycle detection step.
	 *
	 * Getting every edge one more time, if dist[v] > dist[u] + weight(u,v)
	 * we have a negative cycle in the graph. (every time we run the
	 * algorithm, by choosing the negative cycle we will get a
	 * shorter path).
	 ***************************************************************/
	for (int src = 0; src < graph->nodes; src++) {
		for (int dst = 0; dst < graph->nodes; dst++) {
			if (!graph->mat[src][dst])
				continue;

			if (dist[src] == INT_MAX)
				continue;

			if (dist[dst] > dist[src] + graph->mat[src][dst]) {
				*has_neg_cycle = true;
				goto dump;
			}
		}
	}

	*has_neg_cycle = false;

dump:
	/****************************************************************
	 * Dump distances from src.
	 ***************************************************************/
	for (int node = 0; node < graph->nodes; node++)
		printf("[%s] dist(%d; %d) = %d\n", __func__, s, node, dist[node]);

	free(dist);

	return 0;
}

