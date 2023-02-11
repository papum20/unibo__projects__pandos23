#include "pcb.h"
 
/*
       LOCAL (STATIC) VARIABLES:
       pcbFree_table[MAX_PROC];
       /* array of free or unused PCBs
       pcbFree_h;
       /*list of free or unused PCBs
*/



void initPcbs()
{
	static pcb_t pcbFree_table[MAXPROC];
	static pcb_t *pcbFree_h;

	INIT_LIST_HEAD(&(pcbFree_h->p_list));
	pcb_t *p_pcb;
	for(p_pcb = pcbFree_table; p_pcb < pcbFree_table + MAXPROC; p_pcb++)
		list_add_tail(&(p_pcb->p_list), &(pcbFree_h->p_list));
}


//// ALLOCATION

void freePcb(pcb_t *p);

pcb_t *allocPcb();




//// LIST

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