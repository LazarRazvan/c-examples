/** C constructor/destructor use case.
  * Copyright (C) 2026 Lazar Razvan
  */

#include "fn_table.h"

#include <stdio.h>


/*****************************************************************************/

static void fn_table_test2_cb(void)
{
	printf("Handler %s called\n", __func__);
}

__attribute__((constructor))
static void fn_table_test2_register(void)
{
	fn_table_register("test2", fn_table_test2_cb);
}
