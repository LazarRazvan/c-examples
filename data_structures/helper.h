#include "stdio.h"
#include "string.h"

#define OPT_SIZE	10

#define TEST_BIT_MSG	"-- TEST_BIT = "
#define TEST_UNION_MSG	"-- TEST_UNION = "
#define TEST_STRUCT_MSG	"-- TEST_STRUCT = "
#define TEST_EXIT	"-- TEST_EXIT = !(1,2,3)"

#define TEST_BIT	'1'
#define TEST_UNION	'2'
#define TEST_STRUCT	'3'

/* === Bit field example === */
struct bit_field {
	int flags:4;		/* use only 4 bits */
	int options:4;		/* use only 4 bits */
	int fl_number:10;	/* use only 10 bits */
	int op_number:10;	/* use only 10 bits */
};

struct no_bit_field {
	int flags;
	int options;
	int fl_number;
	int fl_options;
};

/* === Union example === */
union options {
	char option_id;
	int option_value;
	char opt_name[OPT_SIZE];
};

/* === Padding inside structures === */

/* Best practice is to add members inside structures descending
 * related to the padding required. (See no_padding example)
 */
struct no_padding {
	int a;
	short b;
	char c;
	/* One byte for alingment */
};

struct padding {
	char a;
	/* 3 bytes padding */
	int b;
	short c;
	/* 2 bytes for alignment */
};
