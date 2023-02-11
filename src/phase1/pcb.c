 #include "pcb.h"
 
/*
       LOCAL (STATIC) VARIABLES:
       pcbFree_table[MAX_PROC];
       * array of free or unused PCBs
       pcbFree_h;
       *list of free or unused PCBs
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

void insertChild(pcb_t *prnt, pcb_t *p){
	p->p_parent=prnt;  //p ha come padre prnt
	list_del(&p->p_sib);
	list_add(&p->p_sib,&prnt->p_child); //p ha come fratello il figlio di prnt in testa e prnt ha come figlio p
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
	}
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



	/*
	struct pcb_t *pos;
	list_for_each_entry(pos, &p->p_parent->p_child, p_child){ 
		if(pos==p){                //p in mezzo alla lista dei figli del suo padre
			list_del(&p->p_sib);
			list_del(p->p_parent);   
			return p;
		}
	}

	*/
}

/* Rimuove il PCB puntato da p dalla lista dei figli del padre.
	Se il PCB puntato da p non ha un padre, restituisce NULL,
	altrimenti restituisce l’elemento rimosso (cioè p). A differenza della
	removeChild, p può trovarsi in una posizione arbitraria (ossia non è
	necessariamente il primo figlio del padre).*/