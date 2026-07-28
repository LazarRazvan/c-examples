/** C constructor/destructor use case.
  * Copyright (C) 2026 Lazar Razvan
  */

#include "fn_table.h"

#include <stdio.h>


/*****************************************************************************/

static void fn_table_test1_cb(void)
{
	printf("Handler %s called\n", __func__);
}

__attribute__((constructor))
static void fn_table_test1_register(void)
{
	fn_table_register("test1", fn_table_test1_cb);
}
