/** Array of functions
  * Copyright (C) 2020 Lazar Razvan
  *
  * This software show how to implement an array of functions
  */

#include "stdio.h"
#include "stdlib.h"

static void my_function1(void)
{
	printf("%s is called\n", __func__);
}

static void my_function2(void)
{
	printf("%s is called\n", __func__);
}

static void my_function3(void)
{
	printf("%s is called\n", __func__);
}

static void my_function4(void)
{
	printf("%s is called\n", __func__);
}

static void (*random_functions[]) (void) = {
	&my_function1,
	&my_function2,
	&my_function3,
	&my_function4
};

int main() {
	void (*f_pointer)(void);
	random_functions[1]();
	f_pointer = random_functions[2];
	f_pointer();
	return 0;
}
