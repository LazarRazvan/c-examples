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

int x;
/* Expose how you can walk through labels that are stored inside an array */
void jump_labels_array(int n)
{
	__label__ label0;
	void g(int a) {
		goto label0;
	}
	static int h;
	int label = 0;
	const void * jumptable[] = {
		&&label1,
		&&label2,
		&&label3,
		&&label4,
		&&label5,
		&&label0
	};
	int arr[n + 5];

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
	arr[0] = arr[0];
	printf("out");
	g(++h);
	printf("never happens");
label0:
	;
}

int main()
{
	jump_labels_array(4);
	return 0;
}
