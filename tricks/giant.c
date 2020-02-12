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

#include "giant.h"

/* TODO: Add resources here
 * Since we want to create a giant process (virtual memory) we need to inspect
 * the limits for stack size and for address space
 */
int resources[] = {RLIMIT_AS, RLIMIT_STACK};

/* TODO: Populate before calling set_limits
 * This array will hold the values for coresponding resources.
 * Default, all are set to infinity
 * limits[i] = limits to set for resource[i]
 */
unsigned long limits[] ={RLIM_INFINITY, RLIM_INFINITY};

/*
 * Return resource as string for printing. This is not very optimal but is
 * called from "get_resources" to map the results to the resource.
 */
char *print_limit(int resource)
{
	switch (resource) {
	case RLIMIT_AS:
		return "RLIMIT_AS";
	case RLIMIT_CORE:
		return "RLIMIT_CORE";
	case RLIMIT_CPU:
		return "RLIMIT_CPU";
	case RLIMIT_DATA:
		return "RLIMIT_DATA";
	case RLIMIT_FSIZE:
		return "RLIMIT_FSIZE";
	case RLIMIT_LOCKS:
		return "RLIMIT_LOCKS:";
	case RLIMIT_MEMLOCK:
		return "RLIMIT_MEMLOCK";
	case RLIMIT_MSGQUEUE:
		return "RLIMIT_MSGQUEUE";
	case RLIMIT_NICE:
		return "RLIMIT_NICE";
	case RLIMIT_NPROC:
		return "RLIMIT_NPROC";
	case RLIMIT_RSS:
		return "RLIMIT_RSS";
	case RLIMIT_RTPRIO:
		return "RLIMIT_RTPRIO";
	case RLIMIT_RTTIME:
		return "RLIMIT_RTTIME";
	case RLIMIT_SIGPENDING:
		return "RLIMIT_RTTIME";
	case RLIMIT_STACK:
		return "RLIMIT_STACK";
	default:
		return "unknown";
	}
	return NULL;
}

/*
 * Inspect the resource limits.
 * RLIMIT_AS :		process maximum virtual memory
 * RLIMIT_STACK :	process maximum stack size
 */
void get_limit(void)
{
	int i;
	struct rlimit r;

	printf("%-35s%-35s%-35s\n", "RESOURCE", "SOFT LIMIT(B)", "HARD LIMIT(B)");
	for (i = 0; i < ARRAY_SIZE(resources); i++) {
		if (getrlimit(resources[i], &r)) {
			fprintf(stderr, "%s [%d:%s]\n", __func__, errno, strerror(errno));
			exit(-1);
		}
		printf("%-35s%-35lu%-35lu\n", print_limit(resources[i]), r.rlim_cur,
					      r.rlim_max);
	}
}

/*
 * Set resources limits. Change only soft limit.
 */
void set_limit(void)
{
	int i;
	struct rlimit r;

	for (i = 0; i < ARRAY_SIZE(resources); i++) {
		r.rlim_cur = limits[i];
		if (setrlimit(resources[i], &r)) {
			fprintf(stderr, "%s [%d:%s]\n", __func__, errno, strerror(errno));
			exit(-1);
		}
	}
}

/*
 * Create a giant process.
 *
 * When allocing a large memory size, mmap is  called.
 * Since we don't alterate the pages (no write), the virtual
 * memory of the process can increase up to address space max
 * soft limit.
 *
 * Note that although pages are not yet maped to frames, due to
 * the large virtual memory, resident memory will increase for
 * keeping the page table of the process.
 */
void create_giant(void)
{
	void *chunks[SIZE];
	int i;

	for (i = 0; i < SIZE; i++) {
		chunks[i] = (void *) malloc(CHUNK_SIZE);
		if (!chunks[i]) {
			fprintf(stderr, "fail on malloc [%d:%s]\n", errno, strerror(errno));
			return;
		}
	}
}

int main()
{
	/* initials limits on the system */
	get_limit();
	/* set stack size and address space to INF */
	set_limit();
	/* limits after change */
	get_limit();
	create_giant();
	/*
         * Create the giant process. Check top..
	 * Example :
	 *   PID USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND
	 * 21196 razvan    20   0  8,000t 8,017g   1236 S   0,0 25,6   0:05.07 giant
	 */
	sleep(50);
	return 0;
}

