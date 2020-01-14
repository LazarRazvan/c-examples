#include <stdio.h>
#include <stdlib.h>

struct device {
	int if_index;
	int addr;
};

#define DEV_GET_IDX(dev)	(dev).if_index
#define DEV_GET_ADR(dev)	(dev).addr
#define DEV_SET_IDX(dev,idx)	(dev).if_index = idx
#define DEV_SET_ADR(dev, adr)	(dev).addr = adr
