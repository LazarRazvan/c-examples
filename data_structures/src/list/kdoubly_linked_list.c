/**
 * Generic singly linked list implementation.
 * Copyright (C) 2023 Lazar Razvan.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "list/kdoubly_linked_list.h"

/******************************** PUBLIC API **********************************/

/**
 * Initialize list head.
 *
 * @head: Generic doubly linked list head.
 */
void kdlist_head_init(kdlist_head_t *head)
{
	head->next = head;
	head->prev = head;
}

/**
 * Check if list is empty.
 *
 * @head: Generic doubly linked list head.
 *
 * Return true if list is empty, otherwise return error.
 */
bool kdlist_is_empty(kdlist_head_t *head)
{
	return head->next == head;
}

/**
 * Check if node is the first in list
 *
 * @head: Generic doubly linked list head.
 * @node: Doubly linked list node.
 *
 * Return true if node is the first in list and false otherwise.
 */
bool kdlist_is_first(kdlist_head_t *head, kdlist_node_t *node)
{
	return head->next == node;
}

/**
 * Check if node is the last in list.
 *
 * @head: Generic doubly linked list head.
 * @node: Doubly linked list node.
 *
 * Return true if node is the last in list and false otherwise.
 */
bool kdlist_is_last(kdlist_head_t *head, kdlist_node_t *node)
{
	return head->prev == node;
}

/**
 * Return the first node in the list without altering the list.
 *
 * @head: Generic doubly linked list head.
 */
kdlist_node_t *kdlist_get_head(kdlist_head_t *head)
{
	return head->next;
}

/**
 * Return the last node in the list without altering the list.
 *
 * @head: Generic doubly linked list head.
 */
kdlist_node_t *kdlist_get_tail(kdlist_head_t *head)
{
	return head->prev;
}

/**
 * Return the first node in the list and extract it from the list.
 *
 * @head: Generic doubly linked list head.
 *
 * If list is empty, return NULL.
 */
kdlist_node_t *kdlist_pop_head(kdlist_head_t *head)
{
	kdlist_node_t *node;

	if (kdlist_is_empty(head))
		return NULL;

	node = head->next;

	// update links
	head->next = node->next;
	node->next->prev = head;

	return node;
}

/**
 * Return the last node in the list and extract it from the list.
 *
 * @head: Generic doubly linked list head.
 *
 * If list is empty, return NULL.
 */
kdlist_node_t *kdlist_pop_tail(kdlist_head_t *head)
{
	kdlist_node_t *node;

	if (kdlist_is_empty(head))
		return NULL;

	node = head->prev;

	// update links
	head->prev = node->prev;
	node->prev->next = head;

	return node;
}

/**
 * Add a node to the head of the list.
 *
 * @head: Generic doubly linked list head.
 * @node: Doubly linked list node.
 */
void kdlist_push_head(kdlist_head_t *head, kdlist_node_t *node)
{
	if (kdlist_is_empty(head)) {
		head->next = node;
		head->prev = node;
		node->next = head;
		node->prev = head;

		return;
	}

	head->next->prev = node;
	node->next = head->next;
	head->next = node;
	node->prev = head;
}

/**
 * Add a node to the tail of the list.
 *
 * @head: Generic doubly linked list head.
 * @node: Doubly linked list node.
 */
void kdlist_push_head(kdlist_head_t *head, kdlist_node_t *node)
{
	if (kdlist_is_empty(head)) {
		head->next = node;
		head->prev = node;
		node->next = head;
		node->prev = head;

		return;
	}

	head->prev->next = node;
	node->prev = head->prev;
	node->next = head;
	head->prev = node;
}

/**
 * Add node after a give node.
 *
 * @base: Base list node.
 * @node: Node to be added.
 */
void kdlist_push_after(kdlist_node_t *base, kdlist_node_t *node)
{
	base->next->prev = node;
	node->prev = base;
	node->next = base->next;
	base->next = node;
}
