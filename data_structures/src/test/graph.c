/**
 * Graph Adjancency Matric test.
 * Copyright (C) 2022 Lazar Razvan.
 */
#include <stddef.h>
#include <stdio.h>
#include <assert.h>

#include "graph/graph.h"

/*********************************** TESTS ************************************/

static int __graph_basic_test(void)
{
	int rv = 0;
	graph_t *graph = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/****************************************************************
	 * Graph create.
	 *     1  -  3
	 *   / |      |
	 * 0   |      |
	 *   \ |      |
	 *     2   -  4
	 ***************************************************************/
	graph = graph_create(5);
	if (!graph) {
		rv = -1; goto error;
	}

	// Add edges
	assert(!graph_add_undirected_edge(graph, 0, 1));
	assert(!graph_add_undirected_edge(graph, 0, 2));
	assert(!graph_add_undirected_edge(graph, 1, 2));
	assert(!graph_add_undirected_edge(graph, 1, 3));
	assert(!graph_add_undirected_edge(graph, 2, 4));
	assert(!graph_add_undirected_edge(graph, 3, 4));

	/****************************************************************
	 * BFS traversal.
	 ***************************************************************/
	printf("Graph BFS:\n");
	assert(!graph_bfs(graph, 0));

	/****************************************************************
	 * DFS traversal.
	 ***************************************************************/
	printf("Graph DFS:\n");
	assert(!graph_dfs(graph, 0));

	/****************************************************************
	 * Destroy graph.
	 ***************************************************************/
	graph_destroy(graph);

	return 0;

error:
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}


static int __graph_node_levels_test(void)
{
	int rv = 0;
	char level;
	graph_t *graph = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/****************************************************************
	 * Graph create.
	 *     1  -  3
	 *   / |      |
	 * 0   |      |
	 *   \ |      |
	 *     2   -  4
	 ***************************************************************/
	graph = graph_create(5);
	if (!graph) {
		rv = -1; goto error;
	}

	// Add edges
	assert(!graph_add_undirected_edge(graph, 0, 1));
	assert(!graph_add_undirected_edge(graph, 0, 2));
	assert(!graph_add_undirected_edge(graph, 1, 2));
	assert(!graph_add_undirected_edge(graph, 1, 3));
	assert(!graph_add_undirected_edge(graph, 2, 4));
	assert(!graph_add_undirected_edge(graph, 3, 4));

	/****************************************************************
	 * Compute level for each node.
	 ***************************************************************/
	assert(!graph_node_level(graph, 0, &level));
	printf("Graph node %d level = %d\n", 0, level);
	assert(!graph_node_level(graph, 1, &level));
	printf("Graph node %d level = %d\n", 1, level);
	assert(!graph_node_level(graph, 2, &level));
	printf("Graph node %d level = %d\n", 2, level);
	assert(!graph_node_level(graph, 3, &level));
	printf("Graph node %d level = %d\n", 3, level);
	assert(!graph_node_level(graph, 4, &level));
	printf("Graph node %d level = %d\n", 4, level);
	assert(!graph_node_level(graph, 5, &level));
	printf("Graph node %d level = %d\n", 5, level);

	/****************************************************************
	 * Destroy graph.
	 ***************************************************************/
	graph_destroy(graph);

	return 0;

error:
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

static int __graph_has_cycles_test(void)
{
	int rv = 0;
	bool has_cycle;
	graph_t *graph1 = NULL, *graph2 = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/****************************************************************
	 * Graph1 create.
	 *
	 *     1  -  3
	 *   / |      |
	 * 0   |      |
	 *   \ |      |
	 *     2   -  4
	 *
	 * Graph2 create
	 *
	 *    1 - 2
	 *  / |
	 * 0  | 3
	 *    |/
	 *    4 - 5
	 ***************************************************************/
	graph1 = graph_create(5);
	if (!graph1) {
		rv = -1; goto error;
	}

	graph2 = graph_create(6);
	if (!graph2) {
		graph_destroy(graph1);
		rv = -2; goto error;
	}

	// (graph1) Add edges
	assert(!graph_add_undirected_edge(graph1, 0, 1));
	assert(!graph_add_undirected_edge(graph1, 0, 2));
	assert(!graph_add_undirected_edge(graph1, 1, 2));
	assert(!graph_add_undirected_edge(graph1, 1, 3));
	assert(!graph_add_undirected_edge(graph1, 2, 4));
	assert(!graph_add_undirected_edge(graph1, 3, 4));

	// (graph2) Add edges
	assert(!graph_add_undirected_edge(graph2, 0, 1));
	assert(!graph_add_undirected_edge(graph2, 1, 2));
	assert(!graph_add_undirected_edge(graph2, 1, 4));
	assert(!graph_add_undirected_edge(graph2, 3, 4));
	assert(!graph_add_undirected_edge(graph2, 4, 5));

	/****************************************************************
	 * Detect cycles for each graph.
	 ***************************************************************/
	assert(!graph_detect_cycle(graph1, &has_cycle));
	printf("Graph1 %s cycles\n", has_cycle ? "has" : "doesn't have");
	assert(!graph_detect_cycle(graph2, &has_cycle));
	printf("Graph2 %s cycles\n", has_cycle ? "has" : "doesn't have");

	/****************************************************************
	 * Destroy graph.
	 ***************************************************************/
	graph_destroy(graph1);
	graph_destroy(graph2);

	return 0;

error:
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

static int __graph_get_length_test(void)
{
	int rv = 0, max_len;
	graph_t *graph = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/****************************************************************
	 * Graph create
	 *      (4)
	 *     1 - 2
	 * (3)/| 
	 * 0   |(2) 
	 *     |(5)
	 *     5 - 4
	 *     \ (6)
	 *      3
	 *
	 * Note that graph has to be an connected undirected graph with
	 * no cycles.
	 ***************************************************************/
	graph = graph_create(6);
	if (!graph) {
		rv = -1; goto error;
	}

	// Add edges
	assert(!graph_add_undirected_wedge(graph, 0, 1, 3));
	assert(!graph_add_undirected_wedge(graph, 1, 2, 4));
	assert(!graph_add_undirected_wedge(graph, 1, 5, 2));
	assert(!graph_add_undirected_wedge(graph, 3, 5, 6));
	assert(!graph_add_undirected_wedge(graph, 4, 5, 5));

	/****************************************************************
	 * Get length for each (u, v) nodes pair.
	 ***************************************************************/
	assert(!graph_max_length(graph, &max_len));
	printf("Graph max length is %d\n", max_len);

	/****************************************************************
	 * Destroy graph.
	 ***************************************************************/
	graph_destroy(graph);

	return 0;

error:
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

static int __graph_bellman_ford_test(void)
{
	int rv = 0;
	graph_t *graph = NULL;
	bool has_negative_cycle;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/****************************************************************
	 * Graph create
	 *
	 * https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm
	 ***************************************************************/
	graph = graph_create(5);
	if (!graph) {
		rv = -1; goto error;
	}

	// Add edges
	assert(!graph_add_directed_wedge(graph, 0, 1, 6));
	assert(!graph_add_directed_wedge(graph, 0, 2, 7));
	assert(!graph_add_directed_wedge(graph, 1, 2, 8));
	assert(!graph_add_directed_wedge(graph, 1, 3, 5));
	assert(!graph_add_directed_wedge(graph, 1, 4, -4));
	assert(!graph_add_directed_wedge(graph, 2, 3, -3));
	assert(!graph_add_directed_wedge(graph, 2, 4, 9));
	assert(!graph_add_directed_wedge(graph, 3, 1, -2));
	assert(!graph_add_directed_wedge(graph, 4, 0, 2));
	assert(!graph_add_directed_wedge(graph, 4, 3, 7));

	/****************************************************************
	 * Bellman Ford algorithm with source node 0.
	 ***************************************************************/
	assert(!graph_bellman_ford(graph, 0, &has_negative_cycle));
	printf("[%s] %s negative cycle\n", __func__,
									has_negative_cycle ?"Has":"Doesn't have");

	/****************************************************************
	 * Destroy graph.
	 ***************************************************************/
	graph_destroy(graph);

	return 0;

error:
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

static int __graph_nlength_cycles_test(void)
{
	int rv = 0;
	graph_t *graph = NULL;

	printf("==============================================================\n");
	printf("%s\n", __func__);
	printf("==============================================================\n");

	/****************************************************************
	 * Graph1 create.
	 *
	 *      0  -  1
	 *      \ ____/  \
	 *      /\        2
	 *      | ----\  /
	 *      4  -  3
	 ***************************************************************/
	graph = graph_create(5);
	if (!graph) {
		rv = -1; goto error;
	}

	// Add edges
	assert(!graph_add_undirected_edge(graph, 0, 1));
	assert(!graph_add_undirected_edge(graph, 0, 3));
	assert(!graph_add_undirected_edge(graph, 1, 2));
	assert(!graph_add_undirected_edge(graph, 1, 4));
	assert(!graph_add_undirected_edge(graph, 2, 3));
	assert(!graph_add_undirected_edge(graph, 3, 4));

	/****************************************************************
	 * Detect number of n length cycles for graph.
	 ***************************************************************/
	printf("Graph has %d cycles of length 4\n", graph_nlength_cycles(graph, 4));

	/****************************************************************
	 * Destroy graph.
	 ***************************************************************/
	graph_destroy(graph);

	return 0;

error:
	printf("[%s] Error: %d\n", __func__, rv);

	return rv;
}

/******************************************************************************/

int main()
{
	int rv = 0;

	/**
	 * Basic test.
	 */
	if (__graph_basic_test()) {
		rv = -1; goto error;
	}

	/**
	 * Node levels test.
	 */
	if (__graph_node_levels_test()) {
		rv = -2; goto error;
	}

	/**
	 * Cycle test.
	 */
	if (__graph_has_cycles_test()) {
		rv = -3; goto error;
	}

	/**
	 * Get length test.
	 */
	if (__graph_get_length_test()) {
		rv = -4; goto error;
	}

	/**
	 * Bellman Ford algorithm.
	 */
	if (__graph_bellman_ford_test()) {
		rv = -5; goto error;
	}

	/**
	 * n length cycles.
	 */
	if (__graph_nlength_cycles_test()) {
		rv = -6; goto error;
	}

	return 0;

error:
	printf("Graph error %d\n", rv);

	return rv;
}
