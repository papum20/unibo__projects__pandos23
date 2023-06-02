#include "exceptions_help.h"

#include "env_nucleus.h"
#include "pcb_help.h"



void __terminateTree(pcb_t *p) {

	if(p == proc_curr)
		/* p is the current process */
		proc_curr = NULL;
	
	if(outBlocked(p)) {
		/* p is blocked on (some) semaphore */
		/* remove p from its semaphore */

		if(is_DEV_SEM(p->p_semAdd))
			/*	if p was blocked on a device semaphore,
				its value will be adjusted by an interrupt laters */
			proc_soft_blocked_n--;
		else
			/*	if p was not blocked on a device semaphore,
				adjust its key to keep the binary semaphore invariant */
			SEM_ADJUST(p->p_semAdd);
	}
	else
		/* p is in the ready state */
		outProcQ(&readyQ, p);

	/* terminate children */
	struct pcb_t *pos;
	pcb_for_each_child(pos, p)
		__terminateTree(pos);
	
	freePcb(p); 
	proc_alive_n--;

}
