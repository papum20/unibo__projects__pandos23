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
#include "linux_list.h"
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


/*	helper function for allcPcb,
	initiates Pcb's values to default.
*/
HIDDEN inline void __initPcb(pcb_t *p)
{
    //process queue
    INIT_LIST_HEAD(&p->p_list);
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



//// LISTS

/* doesn't check if list is empty
*/
HIDDEN inline pcb_t *__removeProcQ(struct list_head* head)
{
	pcb_t *p = container_of(head->next, pcb_t, p_list);
	list_del(head->next);
	return p;
}


//// TREES



#endif