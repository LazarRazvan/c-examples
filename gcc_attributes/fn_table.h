/** C constructor/destructor use case.
  * Copyright (C) 2026 Lazar Razvan
  */


/*****************************************************************************/

#define FN_TABLE_MAX_SIZE			32


/*****************************************************************************/

// function pointer
typedef void (*fn_table_func)(void);

// functions table
typedef struct fn_table_s {

	const char		*name;		// function name
	fn_table_func	func;		// function handler

} fn_table_t;


/*****************************************************************************/

void fn_table_register(const char *name, fn_table_func fn);
void fn_table_run(const char *name);
