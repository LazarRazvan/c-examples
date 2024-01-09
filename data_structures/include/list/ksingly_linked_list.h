/**
 * Generic singly linked list implementation.
 * Copyright (C) 2023 Lazar Razvan.
 */

#include <stdbool.h>

/****************************** DATA STRUCTURE *******************************/

typedef struct kslist_node_s {

	struct kslist_node_s *next;

} kslist_node_t, kslist_head_t;


/********************************** MACROS ***********************************/

//
#define container_of(ptr, type, member) ({					\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type,member) );})


/*****************************************************************************/

// initialize head
#define KSLIST_HEAD_INIT(name)	{ NULL }

// declare and initialize head
#define KSLIST_HEAD(name)	\
	kslist_head_t name = KSLIST_HEAD_INIT(name)

// get struct for entry
#define kslist_entry(ptr, type, member)	\
	container_of(ptr, type, member)

// list iterator
#define kslist_for_each(pos, head)	\
	for (pos = (head)->next; pos; pos = pos->next)

// list iterator for a given position
#define kslist_for_each_continue(pos, head)	\
	for (pos = (pos)->next; pos; pos = pos->next)

// list iterator safe against removal
#define kslist_for_each_safe(pos, n, head)	\
	for (pos = (head)->next, n = pos->next; pos; pos = n, n = pos->next)


/********************************* CALLBACKS *********************************/

//
typedef void (*print_cb)(kslist_node_t *);


/******************************** PUBLIC API *********************************/

//
bool kslist_is_empty(kslist_head_t *head);
bool kslist_is_first(kslist_head_t *head, kslist_node_t *node);
bool kslist_is_last(kslist_head_t *head, kslist_node_t *node);

//
kslist_node_t *kslist_get_head(kslist_head_t *head);
kslist_node_t *kslist_get_tail(kslist_head_t *head);

//
kslist_node_t *kslist_pop_head(kslist_head_t *head);
kslist_node_t *kslist_pop_tail(kslist_head_t *head);

//
void kslist_push_head(kslist_head_t *head, kslist_node_t *node);
void kslist_push_tail(kslist_head_t *head, kslist_node_t *node);
void kslist_push_after(kslist_node_t *base, kslist_node_t *node);

//
void kslist_replace(kslist_head_t *head, kslist_node_t *old, kslist_node_t *new);

//
void kslist_print(kslist_head_t *head, print_cb);
