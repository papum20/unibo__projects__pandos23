#include "helper/exceptions_help.h"



void tmp__terminateDescendants(pcb_t *p) {

	if(p == proc_curr)
		/* p is the current process */
		proc_curr = NULL;
	
	else if(p->p_semAdd != NULL) {
		/* p is blocked on (some) semaphore */

		if(is_DEV_SEM(p->p_semAdd))
			/*	if p was blocked on a device semaphore,
				its value will be adjusted by an interrupt later */
			proc_soft_blocked_n--;
		else
			/*	if p was not blocked on a device semaphore,
				adjust its key to keep the binary semaphore invariant */
			SEM_ADJUST(p->p_semAdd);

		/* remove p from its semaphore */
		outBlocked(p);
	}
	else
		/* p is in the ready state */
		outProcQ(&readyQ, p);

	/* terminate children */
	pcb_t *pos;
	pcb_for_each_child(pos, p)
		tmp__terminateDescendants(pos);
	
	/*	the parent has to delete its children: doing it in the for loop
		would create problems with the list of siblings. */
	while(!emptyChild(p)) {
		freePcb(removeChild(p));
		proc_alive_n--;
	}
}
