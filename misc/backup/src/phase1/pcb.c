#include "pcb.h"
#include "helper/pcb_help.h"
/* include for debug */
// #include "debug.h"
 


 
/* list of free or unused PCBs */
static struct list_head pcbFree_h;



/*	allocates MAXPROC pcb's declaring a static array
	pcbFree_table[].	
	Then adds these pcb's to the pcbFree_h list.
*/
void initPcbs()
{
	static pcb_t pcbFree_table[MAXPROC];

	INIT_LIST_HEAD(&pcbFree_h);
	pcb_t *p_pcb;
	/*	reverse for used to only evaluate the sum (pcbFree_table + MAXPROC - 1) once;
		consequently, it adds on head, which corresponds to adding in tail
		if iterating in order.
	*/
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
	//__removeProcQ already initializes p_list
	__initPcb_no_plist(p);
	return p;
}



//// LISTS

void mkEmptyProcQ(struct list_head *head){
	INIT_LIST_HEAD(head);
}
	

int emptyProcQ(struct list_head *head){
	return __emptyProcQ(head);
}


void insertProcQ(struct list_head *head, pcb_t *p){ //inserimento in coda
	list_add_tail(&p->p_list, head);
}


pcb_t *headProcQ(struct list_head *head){
	if(__emptyProcQ(head))
		return NULL;

	return container_of(head->next, struct pcb_t, p_list); 
}

	
pcb_t *removeProcQ(struct list_head* head){ //rimozione in testa
	if(__emptyProcQ(head))
		return NULL;

	return __removeProcQ(head);
}


pcb_t *outProcQ(struct list_head *head, pcb_t *p){
	struct list_head *pos;
	//scorre la lista che ha come sentinella head fino a trovare il processo p e lo rimuove dalla lista.
	//è necessario scorrere la lista di head per sapere se p è in essa
	list_for_each(pos, head){ 
		if(pos == &p->p_list){
			list_del_init(&p->p_list);
			return p;
		}
	}
	return NULL; //se p non è presente nella lista di head, la funzione ritorna NULL
}


//// TREES

int emptyChild(pcb_t *p){
	return __emptyChild(p);
}

void insertChild(pcb_t *prnt, pcb_t *p){
	p->p_parent = prnt;  //prnt diventa il padre di p
	list_add_tail(&p->p_sib, &prnt->p_child); //aggiungi p in coda nella lista dei figli di prnt
}


pcb_t *removeChild(pcb_t *p){
	if(__emptyChild(p)) //p non ha figli
		return NULL;
		
	pcb_t *tmp = container_of(p->p_child.next, pcb_t, p_sib); 
	list_del_init(&tmp->p_sib);   //rimuove il primo figlio di p
	tmp->p_parent = NULL;
	return tmp;	
}

pcb_t *outChild(pcb_t *p){
	if(p->p_parent == NULL)  //p non ha padre
		return NULL;

	list_del_init(&p->p_sib);  
	p->p_parent = NULL;
	return p; 
}
