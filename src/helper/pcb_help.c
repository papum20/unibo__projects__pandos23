#include "pcb_help.h"


pcb_t* returnChild(pcb_t *p){
	if(__emptyChild(p)) //p non ha figli
		return NULL;
	pcb_t *tmp = container_of(p->p_child.next, pcb_t, p_sib); 
	return tmp;	
}