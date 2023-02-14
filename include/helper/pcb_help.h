/*
HELPER FUNCTIONS FOR pcb
*/


#ifndef PCB_HELP_H
#define PCB_HELP_H


#include "const.h"
#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
/*	pandos_const.h is included before list.h,
	as the former defines NULL while the latter requires it
	(this is just for the editor, as in execution it doesn't matter) */
#include "list.h"





//// ALLOCATION


#define __DFLT_LIST_HEAD ((struct list_head){NULL, NULL})

#define __DFLT_STATE_T 	((state_t){0, 0, 0, 0, 0, 0, 0})

/*	helper function for allcPcb,
	initiates Pcb's values to default.
*/
HIDDEN inline void __initPcb(pcb_t *p)
{
    //process queue
    p->p_list	= __DFLT_LIST_HEAD;
    //process tree fields
    p->p_parent	= NULL;
	p->p_child	= __DFLT_LIST_HEAD;
    p->p_sib	= __DFLT_LIST_HEAD;
    //process status information
    p->p_s = __DFLT_STATE_T;
	p->p_time = 0;
	//semaphore
    p->p_semAdd = NULL;
    //Namespace list
    for(nsd_t **p_nsd = p->namespaces + NS_TYPE_MAX - 1; p_nsd >= p->namespaces; p_nsd--)
		p_nsd = NULL;
}



//// LISTS

/* doesn't check if list is empty
*/
HIDDEN inline pcb_t *__removeProcQ(struct list_head* head)
{
	pcb_t *p = container_of(head->next, struct pcb_t, p_list);
	list_del(head->next);
	return p;
}


//// TREES



#endif