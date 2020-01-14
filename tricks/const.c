/** Example of using const identifier
  * Copyright (C) 2020 Lazar Razvan
  *
  * Using const identifier on pointers can be tricky
  * because you can apply it both to pointer's data type and
  * also to the address of the pointer
  *
  * Example: (const data_type *) (const p);
  * First parenthesis means that the value from the address stored
  * by the pointer is const. The second paranthesis means that the
  * value of the pointer (an address) is const.
  *
  * Change ALLOW_ERRORS define to 1 if you want to see the errors at
  * compile time.
  */
#include <stdio.h>

/* Change to 1 if you want to see the errors */
#define ALLOW_ERRORS	0

/**
  * Using pointer without any const identifier means that we can
  * both change the pointer value and the value from the address
  * that it points to
  */
void test_const_ex1(void)
{
	int a[3] = {1,2,3};
	int *p = a;
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
	(*p)++; /* change value from addr */
	printf("[%s]p addr = %p, p value = %d\n",__func__, p, *p);
	p++; /* change pointer value */
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
}

/**
  * Using const on pointer means that we can dereference it and
  * change the value.
  */
void test_const_ex2(void)
{
	int a[3] = {1,2,3};
	int const *p = a;
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
#if ALLOW_ERRORS /* can't dereference pointer and change the value */
	(*p)++; /* change value from addr */
	printf("[%s]p addr = %p, p value = %d\n",__func__, p, *p);
#endif
	p++; /* change pointer value */
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
}

/**
  * Using const on name means that we can modify the pointer's value.
  */
void test_const_ex3(void)
{
	int a[3] = {1,2,3};
	int * const p = a;
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
	(*p)++; /* change value from addr */
	printf("[%s]p addr = %p, p value = %d\n",__func__, p, *p);
#if ALLOW_ERRORS /* can't change the pointer's value */
	p++; /* change pointer value */
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
#endif
}

/**
  * Using const on both pointer and name means that we can't change the
  * value from the address that pointer points to or its value.
  */
void test_const_ex4(void)
{
	int a[3] = {1,2,3};
	int const * const p = a;
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
#if ALLOW_ERRORS /* can't change anything */
	(*p)++; /* change value from addr */
	printf("[%s]p addr = %p, p value = %d\n",__func__, p, *p);
	p++; /* change pointer value */
	printf("[%s]p addr = %p, p value = %d\n", __func__, p, *p);
#endif
}

int main()
{
	test_const_ex1();
	printf("\n");
	test_const_ex2();
	printf("\n");
	test_const_ex3();
	printf("\n");
	test_const_ex4();
	return 0;
}
