/**
 * Generic singly linked list implementation.
 * Copyright (C) 2023 Lazar Razvan.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "list/ksingly_linked_list.h"


/******************************** PUBLIC API **********************************/

/**
 * Check if list is empty.
 *
 * @head: Generic singly linked list head.
 *
 * Return true if list is empty, otherwise return error.
 */
bool kslist_is_empty(kslist_head_t *head)
{
	return head->next == NULL;
}

/**
 * Check if node is the first in list
 *
 * @head: Generic singly linked list head.
 * @node: Singly linked list node.
 *
 * Return true if node is the first in list and false otherwise.
 */
bool kslist_is_first(kslist_head_t *head, kslist_node_t *node)
{
	return head->next == node;
}

/**
 * Check if node is the last in list.
 *
 * @head: Generic singly linked list head.
 * @node: Singly linked list node.
 *
 * Return true if node is the last in list and false otherwise.
 */
bool kslist_is_last(kslist_head_t *head, kslist_node_t *node)
{
	kslist_node_t *end = head;

	for (; end->next; end = end->next);

	return end == node;
}

/**
 * Return the first node in the list without altering the list.
 *
 * @head: Generic singly linked list head.
 */
kslist_node_t *kslist_get_head(kslist_head_t *head)
{
	return head->next;
}

/**
 * Return the last node in the list without altering the list.
 *
 * @head: Generic singly linked list head.
 */
kslist_node_t *kslist_get_tail(kslist_head_t *head)
{
	kslist_node_t *end = head;

	for (; end->next; end = end->next);

	return end;
}

/**
 * Return the first node in the list and extract it from the list.
 *
 * @head: Generic singly linked list head.
 *
 * If list is empty, return NULL.
 */
kslist_node_t *kslist_pop_head(kslist_head_t *head)
{
	kslist_node_t *node;

	if (kslist_is_empty(head))
		return NULL;

	node = head->next;

	// update links
	head->next = node->next;

	return node;
}

/**
 * Return the last node in the list and extract it from the list.
 *
 * @head: Generic singly linked list head.
 *
 * If list is empty, return NULL.
 */
kslist_node_t *kslist_pop_tail(kslist_head_t *head)
{
	kslist_node_t *node = head;
	kslist_node_t *prev = NULL;

	if (kslist_is_empty(head))
		return NULL;

	for (; node->next; node = node->next) {
		prev = node;
	}

	// update links
	prev->next = NULL;

	return node;
}

/**
 * Add a node to the head of the list.
 *
 * @head: Generic singly linked list head.
 * @node: Singly linked list node.
 */
void kslist_push_head(kslist_head_t *head, kslist_node_t *node)
{
	if (kslist_is_empty(head)) {
		head->next = node;
		node->next = NULL;

		return;
	}

	node->next = head->next;
	head->next = node;
}

/**
 * Add a node to the tail of the list.
 *
 * @head: Generic singly linked list head.
 * @node: Singly linked list node.
 */
void kslist_push_tail(kslist_head_t *head, kslist_node_t *node)
{
	kslist_node_t *prev = head;

	if (kslist_is_empty(head)) {
		head->next = node;
		node->next = NULL;

		return;
	}

	for (; prev->next; prev = prev->next);

	prev->next = node;
	node->next = NULL;
}

/**
 * Add node after a give node.
 *
 * @base: Base list node.
 * @node: Node to be added.
 */
void kslist_push_after(kslist_node_t *base, kslist_node_t *node)
{
	node->next = base->next;
	base->next = node;
}

/**
 * Replace old node with new node.
 *
 * @old: Node to be replaced.
 * @new: Node to be inserted.
 */
void kslist_replace(kslist_head_t *head, kslist_node_t *old, kslist_node_t *new)
{
	kslist_node_t *prev, *node;

	if (kslist_is_empty(head))
		return;

	prev = head;
	for (node = head->next; node; node = node->next) {
		if (node == old) {
			new->next = old->next;
			prev->next = new;

			return;
		}

		prev = node;
	}
}

/**
 * Print list.
 *
 * @head: Generic singly linked list head.
 */
void kslist_print(kslist_head_t *head, print_cb cb)
{
	kslist_node_t *node;

	if (kslist_is_empty(head))
		return;

	kslist_for_each(node, head)
		cb(node);
}
