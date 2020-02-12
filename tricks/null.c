/* Write/Read from null
 * Copyright (C) 2019 Lazar Razvan
 *
 * This software map the virtual memory of the process, forcing
 * to start from 0 (MAP_FIXED). Also, this space is mapped straight
 * to main memory (MAP_ANOYMOUS).
 *
 * By doing this, we can safely write/read an integer value to
 * null (0 address)
 *
 * You should run this file with privileges (root)
 */
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "errno.h"
#include "sys/mman.h"

/* TODO: Set to 1 to see the crash if mem is not mapped */
#define CRASH_ME	0

void write_to_null(void)
{
	void *p = NULL;

	*(int *)p = 123;
}

void read_from_null(void)
{
	void *p = NULL;

	printf("Value from 0 virtual address: %d\n", *(int *)p);
}

int main()
{
	void *addr;

#if CRASH_ME
	/* if null is not mapped => SIGSEGV */
	write_to_null();
#endif
	/* Map virtual address starting from 0 */
	addr = mmap(0, sizeof(int) , PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED) {
		fprintf(stderr, "%s [%d:%s]\n", __func__, errno, strerror(errno));
		return -1;
	}
	printf("Virt memory mapped from addr = %p\n", addr);

	/* write/read to/from null */
	write_to_null();
	read_from_null();
	munmap(addr, sizeof(int));
	return 0;
}
