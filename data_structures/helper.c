#include "helper.h"

/*
 * Show improvements in size when using bit field structure when possible.
 */
void test_bit_field(void)
{
	/* Needing only 28 bits, it is enough for an int */
	printf("Size of bit field strucure = %ld\n", sizeof(struct bit_field));
	/* 4 * sizeof(int) */
	printf("Size of no bit field structure %ld\n", sizeof(struct no_bit_field));
}

void print_union(union options my_opt)
{
	printf("Printing union ... \n");
	printf("id = %c\nval = %d\nname = %s\n", my_opt.option_id,
						   my_opt.option_value, my_opt.opt_name);
}

/*
 * Show improvements in size when using union instead of structure
 *
 * In our case, size of structure is 12 bytes: 10 bytes from option
 * name and 2 byte padding.
 */
void test_union(void)
{
	union options my_opt;

	/* Size of strucutre is size of the largest member(+ padding) */
	printf("Size of union = %ld\n", sizeof(my_opt));

	/* Since all members share the same memory, we should be careful
	 * when accessing a member. This could be altered by changing
	 * another member value
         */

	/* Be sure that union is empty */
	memset(&my_opt, 0, sizeof(my_opt));

	/* Add option_id */
	my_opt.option_id = 65;

	print_union(my_opt);

	/* Add option_value */
	my_opt.option_value = 68;

	print_union(my_opt);
}

/*
 * Show improvements in size when using alligned structures
 *
 * Explain the total size of the structure and the starting
 * address of each member(in memory) to understand why the
 * size is increasing.
 */
void test_struct_padding(void)
{
	struct no_padding n_str;
	struct padding str;

	printf("Size of optimal structure = %ld\n", sizeof(n_str));
	printf("Size of no optimal structure = %ld\n", sizeof(str));

	printf("Optimal structure layout ...\n");
	printf("Starting addr = %p\n", &n_str);
	printf("a at addr = %p\n", &n_str.a);
	printf("b at addr = %p\n", &n_str.b);
	printf("c at addr = %p\n", &n_str.c);

	printf("No optimal structure layout ...\n");
	printf("Starting addr = %p\n", &str);
	printf("a at addr = %p\n", &str.a);
	printf("b at addr = %p\n", &str.b);
	printf("c at addr = %p\n", &str.c);
}

int main()
{
	char test_no;

	printf("Please select test ...\n %s%c\n %s%c\n %s%c\n %s\n",
		TEST_BIT_MSG, TEST_BIT,
		TEST_UNION_MSG, TEST_UNION,
		TEST_STRUCT_MSG, TEST_STRUCT,
		TEST_EXIT);

	while(scanf("%c", &test_no)) {
		fgetc(stdin);
		switch(test_no) {
		case TEST_BIT:
			printf("Bit field example ...\n");
			test_bit_field();
			break;
		case TEST_UNION:
			printf("Union example ...\n");
			test_union();
			break;
		case TEST_STRUCT:
			printf("Padded structures example ...\n");
			test_struct_padding();
			break;
		default:
			goto exit;
			break;
		}
	}
exit:
	return 0;
}
