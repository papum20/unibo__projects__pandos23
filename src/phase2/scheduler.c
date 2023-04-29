#include "scheduler.h"


/*
	lista dei processi ready
*/
extern struct list_head* head_rd;

struct list_head* getHeadRd()
{
	return head_rd;
}