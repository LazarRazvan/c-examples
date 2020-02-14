/* Ring buffer design for multi processing
 * Copyright (C) 2020 Lazar Razvan
 */
#include "string.h"
#include "errno.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdio.h"

/* Path for semaphore and shared memory dev */
#define SEM_NAME	"ring_semaphore"
#define SHM_NAME	"ring_shared"

#define ON_ERR(x) \
do { \
	fprintf(stderr, "%s [%d: %s\n", __func__, (x), strerror(x)); \
} while(0) \

extern int errno;
