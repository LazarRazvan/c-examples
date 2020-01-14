/** Labels as values
  * Copyright (C) 2020 Lazar Razvan
  *
  * https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html
  *
  * This is an extension of gcc compiler that allows to store labels
  * as values, so you can store them inside of an array.
  */

#include <stdio.h>

/* Get the length of an array */
#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(*(x)))

/* Expose how you can walk through labels that are stored inside an array */
void jump_labels_array(void)
{
	int label = 0;
	const void * jumptable[] = {
		&&label1,
		&&label2,
		&&label3,
		&&label4,
		&&label5
	};

select_label:
	if (label == 5)
		goto out;
	goto *jumptable[label++];

label1:
	printf("label1\n");
	goto select_label;
label2:
	printf("label2\n");
	goto select_label;
label3:
	printf("label3\n");
	goto select_label;
label4:
	printf("label4\n");
	goto select_label;
label5:
	printf("label5\n");
	goto select_label;
out:
	printf("out\n");
}

/* Jump from an inner function */
void jump_inner_func(void)
{
	__label__ out;

	printf("Enter in %s\n", __func__);
	void inner_function(void)
	{
		printf("Enter in %s\n", __func__);
		goto out;
		printf("Exit %s\n", __func__);
	}
	/* Although we call the inner function without leaving it (goto is called)
	 * its stack frame will be distroyed when the outer function will return,
	 * when both stack frames will be distroyed
	 */
	inner_function();

out:
	printf("Exit %s\n", __func__);

}

int main()
{
	jump_labels_array();
	printf("\n");
	jump_inner_func();
	return 0;
}
