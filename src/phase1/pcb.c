 #include "pcb.h"
 #include "stdio.h"
 #include "phase1_files/container_of.h"
 

 
	static pcb_t pcbFree_h;
    /* list of free or unused PCBs */



void initPcbs()
{
	static pcb_t pcbFree_table[MAXPROC];

	INIT_LIST_HEAD(&pcbFree_h.p_list);
	pcb_t *p_pcb;
	//reverse for to only evaluate the plus once
	for(p_pcb = pcbFree_table + MAXPROC - 1; p_pcb >= pcbFree_table; p_pcb--)
		list_add(&p_pcb->p_list, &pcbFree_h.p_list);
}


//// ALLOCATION

void freePcb(pcb_t *p)
{
	list_add_tail(&p->p_list, &pcbFree_h.p_list);
}


#define __DFLT_LIST_HEAD {NULL, NULL}

/*	helper function for allcPcb,
	initiates Pcb's values to default.
*/
static inline void __initPcb(pcb_t *p)
{
    /* process queue  */
    p->p_list = NULL;
    /* process tree fields */
    struct pcb_t    *p_parent; /* ptr to parent	*/
	p->p_child = __DFLT_LIST_HEAD;
    p->p_sib = 
    /* process status information */
    state_t p_s;    /* processor state */
	p->p_time = 0;
    /* Pointer to the semaphore the process is currently blocked on */
    p->p_semAdd = NULL;
    /* Namespace list */
    for()
}

pcb_t *allocPcb()
{
	if(list_empty(&pcbFree_h.p_list))
		return NULL;
	
	__initPcb()
}




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