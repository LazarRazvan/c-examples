#include "pointers.h"

int main()
{
	struct device *dev_d;
	struct device dev_s = {
		.if_index = 10,
		.addr = 200
	};

	/* Alloc device on heap */
	dev_d = (struct device *)malloc(sizeof(*dev_d));
	if (!dev_d)
		return -1;

	dev_d->if_index = 20;
	dev_d->addr = 300;

	/* Use macro to print informations */
	printf("device index = %d; device addr = %d\n",
		DEV_GET_IDX(dev_s), DEV_GET_ADR(dev_s));	
	
	printf("device index = %d; device addr = %d\n",
		DEV_GET_IDX(*dev_d), DEV_GET_ADR(*dev_d));

	/* Change values of statically alocated device */
	DEV_SET_IDX(dev_s, 1);
	DEV_SET_ADR(dev_s, 1000);
	/* Change values of dynamically alocated device */
	DEV_SET_IDX(*dev_d, 1);
	DEV_SET_ADR(*dev_d, 1000);

	/* Use macro to print informations after change*/
	printf("device index = %d; device addr = %d\n",
		DEV_GET_IDX(dev_s), DEV_GET_ADR(dev_s));	
	
	printf("device index = %d; device addr = %d\n",
		DEV_GET_IDX(*dev_d), DEV_GET_ADR(*dev_d));

	free(dev_d);
	return 0;
}
