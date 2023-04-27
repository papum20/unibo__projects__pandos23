#include "scheduler.h"

//Lista dei processi pronti 
LIST_HEAD(ready_queue);

struct list_head* head_rd = &(ready_queue);

struct list_head* getHeadRd()
{
	return head_rd;
}