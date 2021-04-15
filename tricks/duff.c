/**
 * Duff's problem
 * Copyright (C) 2021 Lazar Razvan
 *
 * Duff's problem was to copy 16-bit unsigned integers from an array
 * into a memory-mapped output register.
 *
 * The purpose of this project is to ilustrates a manual loop unrolling
 * implementation.
 */

#include <stdio.h>

/**
 * Static source buffer and destination register.
 */
short src[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
short dst;
short *dst_ptr = &dst;

/**
 * Basic implementation for copy 16 bytes values.
 *
 * @count:	Number of values to be copied.
 */
void basic_copy(unsigned int count)
{
	short *src_ptr = src;

	do {
		*dst_ptr = *src_ptr++;
	} while (--count > 0);
}

/**
 * Improved implementation for copy 16 bytes values.
 *
 * @count:	Numeber of values to be copied.
 *
 * Since dst pointer is not changing like in a normal copy (it's a memory
 * mapped register), if count IS ALWAYS DIVISIBLE by 8, the loop can be
 * unrolled eight-fold.
 *
 * The major limitation of this implementation is that count value
 * should be divisible by 8.
 */
void improved_copy(unsigned int count)
{
	unsigned int n;
	short *src_ptr = src;

	if (count % 8) {
		printf("[%s] count is not divisible by 8!\n", __func__);
		return;
	}

	n = count / 8;

	do {
		*dst_ptr = *src_ptr++;
		*dst_ptr = *src_ptr++;
		*dst_ptr = *src_ptr++;
		*dst_ptr = *src_ptr++;
		*dst_ptr = *src_ptr++;
		*dst_ptr = *src_ptr++;
		*dst_ptr = *src_ptr++;
		*dst_ptr = *src_ptr++;
	} while (--n > 0);
}

/**
 * Duff implementation for copy 16 bytes values.
 *
 * @count:	Numeber of values to be copied.
 *
 * Started from improved_copy Duff managed to find a technique of jumping
 * inside loop body for values that are not divisible by 8. Switch labels
 * correpsond to the reminder of count / 8 inside the loop body.
 *
 * Ex:
 *
 * count = 10.
 *
 * 10 % 8 = 2 => ( start from case 2 )
 * case 2
 * case 1
 * case 0 --> end of do { } while; start from beggining
 * case 7
 * case 6
 * case 5
 * case 4
 * case 3
 * case 2
 * case 1
 */
void duff_copy(unsigned int count)
{
	unsigned int n;
	short *src_ptr = src;

	n = (count + 7) / 8;

	/* Jumping into loop body */
	switch (count % 8) {
		case 0:
			do {
				*dst_ptr = *src_ptr++;
		case 7:
				*dst_ptr = *src_ptr++;
		case 6:
				*dst_ptr = *src_ptr++;
		case 5:
				*dst_ptr = *src_ptr++;
		case 4:
				*dst_ptr = *src_ptr++;
		case 3:
				*dst_ptr = *src_ptr++;
		case 2:
				*dst_ptr = *src_ptr++;
		case 1:
				*dst_ptr = *src_ptr++;
			} while (--n > 0);
	}
}

int main()
{
	unsigned int count;

	/* Process source buffer size */
	count = sizeof(src) / sizeof(src[0]);
	if (!count)
		return 0;

	/* Do basic implementation */
	basic_copy(count);
	
	/* Do improved implementation */
	improved_copy(count);

	/* Do Duff implementation */
	duff_copy(count);

	return 0;
}
