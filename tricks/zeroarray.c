#include "zeroarray.h"

struct hw_addr {
	int addr_size;
	char addr[0];
};

/* Alloc a hardware address with an array of 0 elements */
struct dev_name_new *alloc_dev_name_new(int size, const char *name)
{
	struct dev_name_new *dn;

	/* Create extra space at the end */
	dn = (struct dev_name_new *)malloc(sizeof(*dn) + size);
	if (!dn)
		return NULL;

	dn->name_size = size;
	memcpy(dn->name, name, size);
	return dn;
}

/* Alloc a hardware address using an array of MAX_ADDR inside structure */
struct dev_name_old *alloc_dev_name_old(int size, const char *name)
{
	struct dev_name_old *dn;

	dn = (struct dev_name_old *)malloc(sizeof(*dn));
	if (!dn)
		return NULL;

	dn->name_size = size;
	memcpy(dn->name, name, size);
	return dn;
}

/* Print the name of a device using dev_name_new structure layout */
void print_dev_name_new(struct dev_name_new *dn_new)
{
	/* Although name is name[0] in structure, since we have alloc
	 * space for it, we can iterate in it like in a normal array.
	 * You can also use the version inside if 0, but this demonstrates
	 * that the memory was alloced and used
	 */
	for (int i = 0; i < dn_new->name_size; i++)
		printf("%c", dn_new->name[i]);
	printf("\n");
#if 0
	printf("%s\n", dn_new->name);
#endif
}

/* Print the name of a device using dev_name_old structure layout */
void print_dev_name_old(struct dev_name_old *dn_old)
{
	printf("%s\n", dn_old->name);
}

void free_dev_name_new(struct dev_name_new *dn_new)
{
	free(dn_new);
	dn_new = NULL;
}

void free_dev_name_old(struct dev_name_old *dn_old)
{
	free(dn_old);
	dn_old = NULL;
}

int main()
{
	struct dev_name_new *dn_new;
	struct dev_name_old *dn_old;

	dn_new = alloc_dev_name_new(strlen(DEV_NAME), DEV_NAME);	
	dn_old = alloc_dev_name_old(strlen(DEV_NAME), DEV_NAME);	
	
	/* Note the size differences */
	printf("Size dn_new = %ld\n", sizeof(*dn_new));
	printf("Size dn_old = %ld\n", sizeof(*dn_old));

	print_dev_name_new(dn_new);
	print_dev_name_old(dn_old);

	free_dev_name_new(dn_new);
	free_dev_name_old(dn_old);
	return 0;
}
