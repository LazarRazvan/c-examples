/** Initialize slices of array at declaration
  * Copyright (C) 2019 Lazar Razvan
  *
  * This software show how to intialize slices of an array at
  * at declaration time and work only for arrays that lays on
  * the stack.
  *
  */

#include <stdio.h>

/* Macro for pointer size and slices */
#define SIZE	20
#define SLICE1	0 ... 10
#define SLICE2	11 ... SIZE - 1

int main()
{
	int i;
	/* initialize slices of the array */
	int my_array[SIZE] = {
		[SLICE1] = 1,
		[SLICE2] = 2,
	};

	for (i = 0; i < SIZE; i++)
		printf("array[%d] = %d\n", i, my_array[i]);
	return 0;
}

