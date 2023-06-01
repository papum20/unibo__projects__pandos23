#include "exceptions_help.h"
#include "env_nucleus.h"



void __terminateTree(pcb_t *p) {

		
	if(p == proc_curr)
		/* p is the current process */
		proc_curr = NULL;
	
	if(outBlocked(p)) {
		/* p is blocked on (some) semaphore */
		/* remove p from its semaphore */

		if(is_DEV_SEM(p->p_semAdd))
			proc_soft_blocked_n--;
		else
			/*	if p was blocked on a device semaphore,
				adjust its key to keep the binary semaphore invariant */
			SEM_ADJUST(p->p_semAdd);
	}
	else
		/* p is in the ready state */
		outProcQ(&readyQ, p);

	freePcb(p); 
	proc_alive_n--;

}
