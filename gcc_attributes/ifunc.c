/** C ifuncs.
  * Copyright (C) 2026 Lazar Razvan
  */

#include <stdio.h>

/*****************************************************************************/

// Runtime selector id
#define IFUNC_SELECTOR_ID		10

// Runtime function prototype
typedef void (*runtime_cb)(void);


/*****************************************************************************/

// Runtime implementaions
static void __impl1(void)
{
	printf("Hello from %s\n", __func__);
}

static void __impl2(void)
{
	printf("Hello from %s\n", __func__);
}

static void __impl3(void)
{
	printf("Hello from %s\n", __func__);
}

static void __impldef(void)
{
	printf("Hello from %s\n", __func__);
}


/*****************************************************************************/

runtime_cb runtime_selector(void)
{
	switch (IFUNC_SELECTOR_ID) {
		case 1:
			return __impl1;
		case 2:
			return __impl2;
		case 3:
			return __impl3;
		default:
			break;
	}
	
	return __impldef;
}

void runtime(void) __attribute__((ifunc("runtime_selector")));


/*****************************************************************************/

int main()
{
	runtime();
	return 0;
}
