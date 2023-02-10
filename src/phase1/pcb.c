
#include "../../include/pcb.h"


pcbFree_h;						//list of free or unused PCBs
pcbFree_table[MAX_PROC]; // PCB array, with max size=MAX_PROC


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

void insertChild(pcb_t *prnt, pcb_t *p){
	p->p_parent=prnt;  //p ha come padre prnt
	list_add(&p->p_sib,&prnt->p_child); //p ha come fratello il figlio di prnt in testa e prnt ha come figlio p
}


pcb_t *removeChild(pcb_t *p);

	
pcb_t *outChild(pcb_t *p);