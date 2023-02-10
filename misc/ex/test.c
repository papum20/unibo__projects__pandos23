#include <stdlib.h>
#include <stdio.h>
#include "../../include/pcb.h"

LIST_HEAD(ldata);

struct testitem
{
	int value;
	struct list_head list;
};

int main()
{
	struct testitem *item;
	item = malloc(sizeof(struct testitem));
	item->value = 5;
	printf("la lista è %d",list_empty(&(ldata)));
	list_add(&item->list, ldata.next);
	printf(" la lista è %d",list_empty(&(item->list)));

	/*
	if (hash_test(argc, argv) != 0) {
		perror("hash_test");
		return 1;
	}
*/

	return 0;
}
