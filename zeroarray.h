/** Dynamic size array inside structure
  * Copyright (C) 2019 Lazar Razvan
  *
  * This software show how to use an array of zero elements inside a
  * structure in order to store a device name inside.
  *
  * This presents two versionis, one with an array of MAX_NAME_SIZE
  * elements inside the structure and one that dynamically addapts
  * to it. We don't like to use the first version in the case we should
  * alloc many structures because we will use a lot of memory
  */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define MAX_NAME_SIZE	20	/* Max size of a device name */
#define DEV_NAME	"my_device1"

/** Name will be stored at the end of the structure
  * dev_name_new layout :
  * -----------------------------------------------
  * |name_size| name_size bytes accesed by data   |
  * -----------------------------------------------
  */
struct dev_name_new {
	int name_size;
	char name[0];
};

/* Name will be stored in name array inside structure */
struct dev_name_old {
	int name_size;
	char name[MAX_NAME_SIZE];
};

struct dev_name_new *alloc_dev_name_new(int size, const char *name);
struct dev_name_old *alloc_dev_name_old(int size, const char *name);
void print_dev_name_new(struct dev_name_new *dn_new);
void print_dev_name_old(struct dev_name_old *dn_old);
void free_dev_name_new(struct dev_name_new *dn_new);
void free_dev_name_old(struct dev_name_old *dn_old);
