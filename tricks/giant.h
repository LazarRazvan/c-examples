/* Create a giant process (virtual memory)
 * Copyright (C) 2019 Lazar Razvan
 *
 * This software explain the difference between virtual memory
 * and resident memory. Because this difference we can create
 * a process of 8TB virtual memory because we don't access the
 * mapped pages.
 *
 * Although we don't access the pages, the process use a 8GB
 * resident memory to store the page table.
 */

#include "stdio.h"
#include "string.h"
#include "errno.h"
#include "stdlib.h"
#include "unistd.h"
#include "stdint.h"
#include "sys/resource.h"

#define		_4TB	20
#define		_8TB	21
#define		_16TB	22
#define		_32TB	23

/* TODO: Change size depending on memory size
 *
 * Since the virtual memory of the process will be of TB order, although we
 * don't access the pages, process need to keep the page table (resident).
 * Reduce the size if segmentation fault occur
 */
#define SIZE		(1 << _8TB)	/* Number of allocations of 4MB pages */
#define CHUNK_SIZE	(1 << 22)	/* 4 MB */

#define ARRAY_SIZE(x)	sizeof(x) / sizeof(*x)

extern int errno;
