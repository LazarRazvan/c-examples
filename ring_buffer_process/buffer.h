/* Ring buffer design for multi processing
 * Copyright (C) 2020 Lazar Razvan
 */
#include "ringprocs.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "semaphore"
#include "fcntl.h"

/* Will be in shared memory. Don't use pointers */
struct ring_buffer {
	void		*buffer[];
	unsigned int	head;
	unsigned int	tail;
	int		size;
	int		elem_size;
	sem_t		*r_sem;
};
