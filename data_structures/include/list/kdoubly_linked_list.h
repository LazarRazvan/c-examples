/**
 * Generic doubly linked list implementation.
 * Copyright (C) 2023 Lazar Razvan.
 */

#include <stdbool.h>

/****************************** DATA STRUCTURE *******************************/

typedef struct kdlist_node_s {

	struct kdlist_node_s *next;
	struct kdlist_node_s *prev;

} kdlist_node_t, kdlist_head_t;


/********************************** MACROS ***********************************/

//
#define container_of(ptr, type, member) ({					\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})


/*****************************************************************************/

// initialize head
#define KDLIST_HEAD_INIT(name)	{ .next = &(name), .prev = &(name) }

// declare and initialize head
#define KDLIST_HEAD(name)	\
	kdlist_head_t name = KDLIST_HEAD_INIT(name)

// get struct for entry
#define kdlist_entry(ptr, type, member)	\
	container_of(ptr, type, member)

// list iterator
#define kdlist_for_each(pos, head)	\
	for (pos = (head)->next; pos != head; pos = pos->next)

// list iterator reverse
#define kdlist_for_each_reverse(pos, head)	\
	for (pos = (head)->prev; pos != head; pos = pos->prev)

// list iterator for a given position
#define kdlist_for_each_continue(pos, head)	\
	for (pos = (pos)->next; pos != head; pos = pos->next)

// list iterator for a given position reverse
#define kdlist_for_each_continue_reverse(pos, head)	\
	for (pos = (pos)->prev; pos != head; pos = pos->prev)

// list iterator safe against removal
#define kdlist_for_each_safe(pos, n, head)	\
	for (pos = (head)->next, n = pos->next; pos != head; pos = n, n = pos->next)


/********************************* CALLBACKS *********************************/

//
typedef void (*print_cb)(kdlist_node_t *);


/******************************** PUBLIC API *********************************/

//
void kdlist_head_init(kdlist_head_t *head);

//
bool kdlist_is_empty(kdlist_head_t *head);
bool kdlist_is_first(kdlist_head_t *head, kdlist_node_t *node);
bool kdlist_is_last(kdlist_head_t *head, kdlist_node_t *node);

//
kdlist_node_t *kdlist_get_head(kdlist_head_t *head);
kdlist_node_t *kdlist_get_tail(kdlist_head_t *head);

//
kdlist_node_t *kdlist_pop_head(kdlist_head_t *head);
kdlist_node_t *kdlist_pop_tail(kdlist_head_t *head);

//
void kdlist_push_head(kdlist_head_t *head, kdlist_node_t *node);
void kdlist_push_tail(kdlist_head_t *head, kdlist_node_t *node);
void kdlist_push_after(kdlist_node_t *base, kdlist_node_t *node);
