/** C constructor/destructor use case.
  * Copyright (C) 2026 Lazar Razvan
  */

#include "fn_table.h"

#include <stdio.h>
#include <string.h>


/*****************************************************************************/

//
static fn_table_t fn_table[FN_TABLE_MAX_SIZE];
static int fn_table_count = 0;


/*****************************************************************************/

void fn_table_register(const char *name, fn_table_func fn)
{
	if (fn_table_count >= FN_TABLE_MAX_SIZE) {
		fprintf(stderr, "%s: table full, dropping %s\n", __func__, name);
		return;
	}

	fn_table[fn_table_count].name = name;
	fn_table[fn_table_count].func = fn;
	fn_table_count++;
}

void fn_table_run(const char *name)
{
	for (int i = 0; i < fn_table_count; i++) {
		if (strcmp(fn_table[i].name, name))
			continue;

		fn_table[i].func();
		return;
	}

	printf("%s: no handler found for %s\n", __func__, name);
}
