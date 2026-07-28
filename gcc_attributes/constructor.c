/** C constructor/destructor.
  * Copyright (C) 2026 Lazar Razvan
  */

#include <stdio.h>

/*****************************************************************************/

__attribute__((constructor))
static void __init(void)
{
	printf("Func %s called\n", __func__);
}

__attribute__((destructor))
static void __deinit(void)
{
	printf("Func %s called\n", __func__);
}


/*****************************************************************************/

int main()
{
	printf("Func %s called\n", __func__);
	return 0;
}
