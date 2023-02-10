
#include "../../include/pcb.h"


pcbFree_h;						//list of free or unused PCBs
pcbFree_table[MAXPROC]; // PCB array, with max size=MAX_PROC


//// ALLOCATION


//// LIST

pcb_t *outChild(pcb_t *p)
{
	return NULL;
}

//// TREES
static int emptyChild(pcb_t *p){
	return (list_empty(&(p->p_child)));
}


static void insertChild(pcb_t *prnt, pcb_t *p){
	return;
}
	/* Inserisce il PCB puntato da p come figlio
	del PCB puntato da prnt.*/