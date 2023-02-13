#include "pcb.h"
#include "pcb_help.h"
 


 
static struct list_head pcbFree_h;
/* list of free or unused PCBs */



void initPcbs()
{
	static pcb_t pcbFree_table[MAXPROC];

	INIT_LIST_HEAD(&pcbFree_h);
	pcb_t *p_pcb;
	//reverse for to only evaluate the plus once
	for(p_pcb = pcbFree_table + MAXPROC - 1; p_pcb >= pcbFree_table; p_pcb--)
		list_add(&p_pcb->p_list, &pcbFree_h);
}


//// ALLOCATION

void freePcb(pcb_t *p)
{
	list_add_tail(&p->p_list, &pcbFree_h);
}


pcb_t *allocPcb()
{
	if(list_empty(&pcbFree_h))
		return NULL;

	pcb_t *p = __removeProcQ(&pcbFree_h);
	__initPcb(p);
	return p;
}




//// LISTS

void *mkEmptyProcQ(struct list_head *head);
	

int emptyProcQ(struct list_head *head);


void insertProcQ(struct list_head *head, pcb_t *p);


pcb_t *headProcQ(struct list_head *head);


pcb_t *removeProcQ(struct list_head* head);

	
pcb_t *outProcQ(struct list_head *head, pcb_t *p);




//// TREES
int emptyChild(pcb_t *p){
	return (list_empty(&(p->p_child)));
}

void insertChild(pcb_t *prnt, pcb_t *p);


pcb_t *removeChild(pcb_t *p);

	
pcb_t *outChild(pcb_t *p);