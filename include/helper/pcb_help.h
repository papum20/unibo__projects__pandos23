/*
HELPER FUNCTIONS FOR pcb
*/


#ifndef PCB_HELP_H
#define PCB_HELP_H


#include "const.h"
#include "pandos_const.h"
#include "pandos_types.h"

#include "linux_list.h"
#include "types.h"

/* include for debug */
#include "debug.h"




//// ALLOCATION

/** inits a state_t object to default values (0)
 *	@state: state_t object
 *	note: doing like this was necessary, as by assigning {0,0,...,0},
 *	gcc tried to invoke memset, not defined
 *	not working with stdlib.
*/
#define __DFLT_STATE_T(state) \
	state.entry_hi	= 0;															\
	state.cause		= 0;															\
	state.status	= 0;															\
	state.pc_epc	= 0;															\
	for(unsigned int *_p = state.gpr + STATE_GPR_LEN - 1; _p >= state.gpr; _p--)	\
		*_p = 0;																	\
	state.hi		= 0;															\
	state.lo		= 0;



/* __initPcb, but doesn't init p_list
*/
HIDDEN inline void __initPcb_no_plist(pcb_t *p)
{
    //process tree fields
    p->p_parent	= NULL;
    INIT_LIST_HEAD(&p->p_child);
    INIT_LIST_HEAD(&p->p_sib);
    //process status information
    __DFLT_STATE_T(p->p_s);
	p->p_time = 0;
	//semaphore
    p->p_semAdd = NULL;
    //Namespace list
    for(nsd_t **p_nsd = p->namespaces + NS_TYPE_MAX - 1; p_nsd >= p->namespaces; p_nsd--)
		*p_nsd = NULL;
}

/*	helper function for allcPcb,
	initiates Pcb's values to default.
*/
HIDDEN inline void __initPcb(pcb_t *p)
{
    //process queue
    INIT_LIST_HEAD(&p->p_list);
    //other fields
    __initPcb_no_plist(p);
}



//// LISTS

/**
 * doesn't check if list is empty
*/
HIDDEN inline pcb_t *__removeProcQ(struct list_head* head)
{
	pcb_t *p = container_of(head->next, pcb_t, p_list);
	list_del_init(head->next);
	return p;
}

/**
 * Same as emptyProcQ, but inline.
*/
HIDDEN inline int __emptyProcQ(struct list_head *head) {
	return list_empty(head);
}


//// TREES

HIDDEN inline int __emptyChild(pcb_t *p) {
	return __emptyProcQ(&p->p_child);
}


/**
 * iterate over the children of pcb_t *parent.
 * @pos:	the pcb_t * to use as a loop cursor.
 * @parent:	the pcb_t *parent.
 */
#define pcb_for_each_child(pos, parent) \
	list_for_each_entry(pos, &(parent->p_child), p_sib)





#endif /* PCB_HELP_H */