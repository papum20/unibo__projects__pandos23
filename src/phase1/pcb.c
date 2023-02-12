 #include "pcb.h"
 #include "stdio.h"
 #include "phase1_files/container_of.h"
 

 
	static pcb_t pcbFree_h;
    /* list of free or unused PCBs */



void initPcbs()
{
	static pcb_t pcbFree_table[MAXPROC];

	INIT_LIST_HEAD(&(pcbFree_h.p_list));
	pcb_t *p_pcb;
	//reverse for to only evaluate the plus once
	for(p_pcb = pcbFree_table + MAXPROC - 1; p_pcb >= pcbFree_table; p_pcb--)
		list_add(&(p_pcb->p_list), &(pcbFree_h.p_list));
}


//// ALLOCATION
void freePcb(pcb_t *p);

pcb_t *allocPcb();




//// LIST

void mkEmptyProcQ(struct list_head *head){
	INIT_LIST_HEAD(head);
}
	

int emptyProcQ(struct list_head *head){
	return (list_empty(head));
}


void insertProcQ(struct list_head *head, pcb_t *p){  //inserimento in coda
	list_add_tail(&p->p_list,head);
}


pcb_t *headProcQ(struct list_head *head){
	if(emptyProcQ(head))
		return NULL;
	return container_of(head->next, struct pcb_t, p_list); 
}

	
pcb_t *removeProcQ(struct list_head* head){ //rimozione in testa
	if(emptyProcQ(head))
		return NULL;
	list_del(head->next);
}

	
pcb_t *outProcQ(struct list_head *head, pcb_t *p){
	struct list_head *pos;
	struct pcb_t *tmp;
	list_for_each(pos, head){ //scorre tutta la lista
		tmp = container_of(pos, struct pcb_t, p_list); 
		if(tmp==p){
			list_del(p->p_list);
			return p;
		}	
	}
	return NULL;
}


//// TREES
int emptyChild(pcb_t *p){
	return (list_empty(&(p->p_child)));
}

void insertChild(pcb_t *prnt, pcb_t *p){
	p->p_parent=prnt;  //p ha come padre prnt
	list_add_tail(&p->p_sib,&prnt->p_child); //p ha come fratello il figlio di prnt in testa e prnt ha come figlio p
}


pcb_t *removeChild(pcb_t *p){
	if(emptyChild((p))==1)
		return NULL;
	pcb_t *tmp = p->p_child.next;
	list_del(p->p_child.next);  
	list_del(&tmp->p_sib);
	tmp->p_parent = NULL;
	return tmp;	
}

pcb_t *outChild(pcb_t *p){
	if(p->p_parent==NULL)  //p non ha padre
		return NULL;
	if(p->p_parent->p_child.next == p){  //p è il primo figlio del suo padre
		return removeChild(p);
	}/*
	struct list_head *pos;
	struct pcb_t *tmp;
	list_for_each(pos, &p->p_parent->p_child){ //scorre tutta la lista
		tmp = container_of(pos, struct pcb_t, p_child); 
		if(tmp==p){
			list_del(&p->p_sib);
			p->p_parent = NULL;  
			list_del(pos);
			return p;
		}
	}
	*/
	list_del(&p->p_sib);
	p->p_parent = NULL;
	return p; 
}
