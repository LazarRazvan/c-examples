/** C comparison between function pointers and ifuncs.
  * Copyright (C) 2026 Lazar Razvan
  */

#include <stdio.h>
#include <stdint.h>


/*****************************************************************************/

static inline uint64_t rdtsc(void)
{
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((uint64_t)hi << 32) | lo;
}


/*****************************************************************************/

/**
 * ifunc config.
 */

static volatile int _ifunc_val = 0;

//
typedef void (*ifunc_cb)(void);

//
static void __ifunc_impl(void) { _ifunc_val++; }

//
ifunc_cb ifunc_impl(void) { return __ifunc_impl; }

void ifunc_fn(void) __attribute__((ifunc("ifunc_impl")));


/*****************************************************************************/

/**
 * function pointer config.
 */

static volatile int __fptr_val = 0;

//
typedef void (*fptr_cb)(void);

//
static void __fptr_impl(void) { __fptr_val++; }

//
static volatile fptr_cb fptr_fn = __fptr_impl;


/*****************************************************************************/

/**
 * plain function config.
 */
static volatile int __plain_val = 0;

__attribute__((noinline)) static void __plain_fn(void) { __plain_val++; }


/*****************************************************************************/

#define ITERATIONS		100000000UL
#define WARMUP			10000UL
#define TRIALS			5


/**
 * Test functions.
 */
__attribute__((noinline)) static uint64_t __bench_plain(void)
{
	for (unsigned long i = 0; i < WARMUP; i++) __plain_fn();

	uint64_t best = UINT64_MAX;
	for (int t = 0; t < TRIALS; t++) {
		uint64_t start = rdtsc();
		for (unsigned long i = 0; i < ITERATIONS; i++) __plain_fn();
		uint64_t elapsed = rdtsc() - start;
		if (elapsed < best) best = elapsed;
	}

	return best / ITERATIONS;
}

__attribute__((noinline)) static uint64_t __bench_ifunc(void)
{
	for (unsigned long i = 0; i < WARMUP; i++) ifunc_fn();

	uint64_t best = UINT64_MAX;
	for (int t = 0; t < TRIALS; t++) {
		uint64_t start = rdtsc();
		for (unsigned long i = 0; i < ITERATIONS; i++) ifunc_fn();
		uint64_t elapsed = rdtsc() - start;
		if (elapsed < best) best = elapsed;
	}

	return best / ITERATIONS;
}

__attribute__((noinline)) static uint64_t __bench_fptr(void)
{
	for (unsigned long i = 0; i < WARMUP; i++) fptr_fn();

	uint64_t best = UINT64_MAX;
	for (int t = 0; t < TRIALS; t++) {
		uint64_t start = rdtsc();
		for (unsigned long i = 0; i < ITERATIONS; i++) fptr_fn();
		uint64_t elapsed = rdtsc() - start;
		if (elapsed < best) best = elapsed;
	}

	return best / ITERATIONS;
}


/*****************************************************************************/

int main()
{
	//
	printf("ifunc : %lu cycles/call\n", __bench_ifunc());
	printf("fptr  : %lu cycles/call\n", __bench_fptr());
	printf("plain : %lu cycles/call\n", __bench_plain());

	return 0;
}
