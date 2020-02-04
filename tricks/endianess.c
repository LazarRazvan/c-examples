/** Detect platform endianess
  * Copyright (C) 2020 Lazar Razvan
  */

#include "stdio.h"

#define BIG_ENDIAN 0x12
#define LIT_ENDIAN 0x78

/*
 * Detect platform endianess by checking the value of the first
 * byte in memory
 */
static int check_endianess(int value) {
	char *c1;

	c1 = (char *)&value;

	/* Print bytes order in memory */
	printf("Value 0x%02x in memory at %p : \n", value, &value);
	for (int i = 0; i < sizeof(int); i++) {
		printf("[%p] : byte_%d = 0x%02x\n", c1, i, *c1);
		c1++;
	}

	c1 = (char *)&value;

	/* Detect endianess */
	if (*c1 == BIG_ENDIAN)
		return 0;
	return 1;
}

int main() {
	int res;
	int a = 0x12345678;

	res = check_endianess(a);

	res == 0 ? printf("BIG ENDIAN\n") : printf("LITTLE ENDIAN\n");
	return 0;
}
