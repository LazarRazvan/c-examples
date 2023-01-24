/**
 * Singly linked list implementation.
 * Copyright (C) 2022 Lazar Razvan.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "list/singly_linked_list.h"

/********************************** STATIC ************************************/

/**
 * Allocate memory for a singly linked list node.
 *
 * @data	: Node data.
 *
 * Return node address on success and NULL otherwise.
 */
slist_t *__slist_node_create(void *data)
{
	slist_t *node = NULL;

	node = malloc(sizeof(slist_t));
	if (!node)
		return NULL;

	node->data = data;
	node->next = NULL;

	return node;
}

/******************************** PUBLIC API **********************************/

/********************************** INSERT ************************************/
/**
 * Add node as head of the list. Note that this function will update head
 * address.
 *
 * @head	: Singly linked list head.
 * @data	: New node data.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_insert_first(slist_t **head, void *data)
{
	slist_t *node = NULL;

	// create new node
	node = __slist_node_create(data);
	if (!node)
		return -1;

	// add node as head of the list and update head
	node->next = *head;
	*head = node;

	return 0;
}

/**
 * Add node after a node in list. Note that this function can't update head
 * address.
 *
 * @head		: Singly linked list head.
 * @prev_data	: Previous node data.
 * @new_data	: New node data.
 * @_cb			: Compare callback.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_insert_after(slist_t *head, void *prev_data, void *new_data,
						cmp_cb _cb)
{
	slist_t *new_node, *it;

	// list can't be empty
	if (!head)
		return -1;

	// iterate in list to find previous node
	it = head;
	while (it) {
		if (_cb(it->data, prev_data) == 0)
			break;

		it = it->next;
	}

	// error if previous node not found
	if (!it)
		return -2;

	// add new node after previous node
	new_node = __slist_node_create(new_data);
	if (!new_node)
		return -3;

	new_node->next = it->next;
	it->next = new_node;

	return 0;
}

/**
 * Add node before a node in list. Note that this function may update head
 * address.
 *
 * @head		: Singly linked list head.
 * @after_data	: After node data.
 * @new_data	: New node data.
 * @_cb			: Compare callback.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_insert_before(slist_t **head, void *after_data, void *new_data,
						cmp_cb _cb)
{
	slist_t *new_node, *it, *prev;

	// list can't be empty
	if (!*head)
		return -1;

	// iterate in list to find after node
	prev = NULL;
	it = *head;
	while (it) {
		if (_cb(it->data, after_data) == 0)
			break;

		prev = it;
		it = it->next;
	}

	// error if previous node not found
	if (!it)
		return -2;

	// add new node after previous node
	new_node = __slist_node_create(new_data);
	if (!new_node)
		return -3;

	// If previous is null, update list head
	if (!prev) {
		new_node->next = it;
		*head = new_node;
	} else {
		new_node->next = it;
		prev->next = new_node;
	}

	return 0;
}

/**
 * Add node at end of the list. Note that this function may update head
 * address.
 *
 * @head	: Singly linked list head.
 * @data	: New node data.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_insert_last(slist_t **head, void *data)
{
	slist_t *it, *node = NULL;

	// create new node
	node = __slist_node_create(data);
	if (!node)
		return -1;

	// If list is empty, update head
	if (!*head) {
		*head = node;
		return 0;
	}

	// If list is not empty, go to last element
	it = *head;
	while (it->next)
		it = it->next;

	// Append new element
	node->next = NULL;
	it->next = node;

	return 0;
}

/*********************************** DELETE ***********************************/
/**
 * Delete node from the beginning of the list. Note that this function update
 * head address.
 *
 * @head		: Singly linked list head.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_delete_first(slist_t **head)
{
	slist_t *it;

	// If list is empty, return error
	if (!*head)
		return -1;

	it = *head;
	*head = it->next;

	free(it->data);
	free(it);

	return 0;
}

/**
 * Delete node after a node in list. Note that this function can't update head
 * address.
 *
 * @head		: Singly linked list head.
 * @prev_data	: Previous node data.
 * @_cb			: Compare callback.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_delete_after(slist_t *head, void *prev_data, cmp_cb _cb)
{
	slist_t *node, *it;

	// list can't be empty
	if (!head)
		return -1;

	// iterate in list to find previous node
	it = head;
	while (it) {
		if (_cb(it->data, prev_data) == 0)
			break;

		it = it->next;
	}

	// error if previous node not found or is last element in list
	if (!it || !it->next)
		return -2;

	// remove next element from list
	node = it->next;
	it->next = it->next->next;

	// free memory
	free(node->data);
	free(node);

	return 0;
}

/**
 * Delete node before a node in list. Note that this function may update head
 * address.
 *
 * @head		: Singly linked list head.
 * @after_data	: After node data.
 * @_cb			: Compare callback.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_delete_before(slist_t **head, void *after_data, cmp_cb _cb)
{
	slist_t *it, *prev, *pprev;

	// list can't be empty
	if (!*head)
		return -1;

	// iterate in list to find after node
	pprev = NULL;
	prev = NULL;
	it = *head;
	while (it) {
		if (_cb(it->data, after_data) == 0)
			break;

		pprev = prev;
		prev = it;
		it = it->next;
	}

	// error if after node not found or if after node is head
	if (!it || !prev)
		return -2;

	// If pre-previous is null, update list head
	if (!pprev) {
		*head = it;
	} else {
		pprev->next = it;
	}

	free(prev->data);
	free(prev);

	return 0;
}

/**
 * Delete node from the end of the list. Note that this function may update
 * head address if only one element is in list.
 *
 * @head		: Singly linked list head.
 *
 * Return 0 on success and <0 otherwise.
 */
int slist_delete_last(slist_t **head)
{
	slist_t *it, *prev;

	// If list is empty, return error
	if (!*head)
		return -1;

	// iterate in list to find last element
	prev = NULL;
	it = *head;
	while (it->next) {
		prev = it;
		it = it->next;
	}

	// If previous is null, update list head (empty list)
	if (!prev)
		*head = NULL;
	else
		prev->next = NULL;

	free(it->data);
	free(it);

	return 0;
}

/************************************ GET *************************************/
/**
 * Get n-th node of a list.
 *
 * @head	: Singly linked list head.
 * @n		: Node index.
 *
 * Return node if found and NULL on error.
 */
slist_t *slist_get_nth(slist_t *head, unsigned int n)
{
	unsigned int i = 0;

	while (head && (i < n)) {
		head = head->next;
		i++;
	}

	return head;
}

/*********************************** PRINT ************************************/
/**
 * Print list.
 *
 * @head	: Singly linked list head.
 * @_cb		: Node data print callback.
 */
void slist_print(slist_t *head, print_cb _cb)
{
	if (!head) {
		printf("Empty list!\n");
		return;
	}

	while (head) {
		_cb(head->data);
		head = head->next;
	}
}

/**
 * Reverse print list.
 *
 * @head	: Singly linked list head.
 * @_cb		: Node data print callback.
 */
void slist_print_reverse(slist_t *head, print_cb _cb)
{
	if (!head)
		return;

	slist_print_reverse(head->next, _cb);
	_cb(head->data);
}

/**
 * Print middle element of list.
 *
 * 1->2->3->4->5 => 3.
 * 1->2->3->4->5->6 => 4.
 *
 * @head	: Singly linked list head.
 * @_cb		: Node data print callback.
 */
void slist_print_middle(slist_t *head, print_cb _cb)
{
	slist_t *_slow, *_fast;

	if (!head)
		return;

	_slow = head;
	_fast = head;

	while (_fast && _fast->next) {
		_slow = _slow->next;
		_fast = _fast->next->next;
	}

	_cb(_slow->data);
}

/************************************ LOOP ************************************/
/**
 * Detect if a singly linked list has a loop. Perform Floyd's Cycle Detection
 * algorithm with two pointers (slow and fast pointer).
 *
 * @head	: Singly linked list head.
 *
 * Return true if loop is found and false otherwise.
 */
bool slist_has_loop(slist_t *head)
{
	slist_t *_slow, *_fast;

	if (!head)
		return false;

	_slow = head;
	_fast = head;

	while (_slow && _fast && _fast->next) {
		_slow = _slow->next;
		_fast = _fast->next->next;

		if (_slow == _fast)
			return true;
	}

	return false;
}

/**
 * Get loop starting node for a list (only if loop is found).Perform Floyd's
 * Cycle Detection algorithm with two pointers (slow and fast pointer). If
 * loop is found, move one pointer to head of list and start moving one node
 * each time with both pointers.
 *
 * @head	: Singly linked list head.
 *
 * Return true if loop is found and false otherwise.
 */
slist_t *slist_loop_start(slist_t *head)
{
	slist_t *_slow, *_fast;

	if (!head)
		return false;

	_slow = head;
	_fast = head;

	while (_slow && _fast && _fast->next) {
		_slow = _slow->next;
		_fast = _fast->next->next;

		if (_slow == _fast)
			goto start_loop_detect;
	}

	return false;

start_loop_detect:
	/**
	 * Move one pointer at list head and continue one node at a time with both
	 * _slow and _fast pointers.
	 */
	_slow = head;

	while (_slow != _fast) {
		_slow = _slow->next;
		_fast = _fast->next;
	}

	return _slow;
}

/********************************* PALINDROME *********************************/
/**
 * Detect if a list is palindrom using recursion approach. Stack can also be
 * used.
 *
 * @left	: Left side of list.
 * @right	: Right side of list.
 * @_cb		: Compare callback.
 *
 * Return true if list is palindrome and flase otherwise.
 */
bool slist_is_palindrome(slist_t **left, slist_t *right, cmp_cb _cb)
{
	bool res;

	if (!right)
		return true;

	if (slist_is_palindrome(left, right->next, _cb) == false)
		return false;

	res = _cb((*left)->data, right->data) ? false : true;
	*left = (*left)->next;

	return res;
}

/********************************* DUPLICATES *********************************/
/**
 * Remove duplicate nodes in an ordered list. Note that list has to be sorted.
 *
 * @head	: Singly linked list head.
 * @_cb		: Compare callback.
 *
 * Return 0 on success and <0 otherwise.
 */
void slist_ordered_delete_duplicates(slist_t *head, cmp_cb _cb)
{
	slist_t *it, *prev;

	if (!head || !head->next)
		return;

	it = head->next;
	prev = head;
	while (it) {

		if (_cb(it->data, prev->data) == 0) {
			// remove "it" element and not update "prev"
			prev->next = it->next;

			free(it->data);
			free(it);

			it = prev->next;
		} else {
			// increment "it" and "prev"
			prev = it;
			it = it->next;
		}
	}
}

/*********************************** DESTROY **********************************/
/**
 * Destroy (free space) for a list.
 */
void slist_destroy(slist_t **head)
{
	slist_t *it = *head, *aux;

	if (!it)
		return;

	while (it) {
		aux = it;
		it = it->next;

		free(aux->data);
		free(aux);
	}

	*head = NULL;
}
