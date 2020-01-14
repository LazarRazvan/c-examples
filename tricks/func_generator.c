/** Function generator
  * Copyright (C) 2020 Lazar Razvan
  *
  * This trick can be used if you need a large number of function
  * (i.e configuration) and you don't want to write the down.
  *
  * You can stop the compiling process after preprocessing and
  * inspect the code
  *
  * gcc -E func_generator.c
  *
  */
#include <stdio.h>

/* This macro is used to for generating the function */
#define GENERATE_FUNCTION(id) \
static void generated_func_##id() \
{ \
	printf("Function %s is called\n",  __func__); \
} \

/* This macro is used to call the generated functions */
#define CALL_FUNCTION(id) generated_func_##id()

/* Generate the 3 functions */
GENERATE_FUNCTION(1)
GENERATE_FUNCTION(2)
GENERATE_FUNCTION(3)

int main()
{
	/* Call the 3 functions */
	CALL_FUNCTION(1);
	CALL_FUNCTION(2);
	CALL_FUNCTION(3);
	return 0;
}
