#include "initial.h"
#include "helper/initial_help.h"
#include "env_nucleus.h"


/*
Important Point: When setting up a new processor state one must
set the previous bits (i.e. IEp & KUp) and not the current bits (i.e.
IEc & KUc) in the Status register for the desired assignment to take
effect after the initial LDST loads the processor state. [Section ??-
pops]
*/

/* 1. Declare the Level 3 global variables. */

/* number of started, but not yet terminated processes. */
int proc_alive_n;

/*	number of started, but not terminated, processes, that are
	in the “blocked” state due to an I/O or timer request.
*/
int proc_soft_blocked_n;

/* queue of pcbs that are in the "ready" state. */
struct list_head readyQ;

/*	Pointer to the pcb that is in the “running” state,
	i.e. the current executing process.
*/
pcb_t *proc_curr;	

/*	One integer semaphore for each external (sub)device, plus one
	for the Pseudo-clock, plus four for two, independend terminal
	devices, each needing two semaphores for read and write. 
*/
int dev_sems[N_DEV_SEM];




int main() {




	/* 2. Populate the (Processor 0) Pass Up Vector. */
	
	/*	Set the Nucleus exception and TLB-Refill event handlers,
		and their SP to the top of the Nucleus stack page.
	*/
	PASSUP_VECTOR->tlb_refill_handler	= (memaddr)uTLB_RefillHandler;
	PASSUP_VECTOR->tlb_refill_stackPtr	= (memaddr)KERNELSTACK;
	PASSUP_VECTOR->exception_handler	= (memaddr)Exception_handler;
	PASSUP_VECTOR->exception_stackPtr	= (memaddr)KERNELSTACK;



	/* 3. Initialize the Level 2 data structures */

	initPcbs();
	initASH();
	initNamespaces();



	/* 4. Initialize all Nucleus maintained variables */

	proc_alive_n		= 0;
	proc_soft_blocked_n	= 0;
	mkEmptyProcQ ( &readyQ );
	/* proc_curr used and initialized later */
	
	/* Semaphores set to 0, as they will be used for synchronization. */
	for(int i = 0; i < N_DEV_SEM; i++)
		dev_sems[i] = SEM_VALUE_SYNC;



	/* 5. Load the system-wide Interval Timer. */
	LDIT(INTERVAL_TIMER_INIT);



	/* 6. Instantiate a single process, place its pcb in the Ready Queue, and
	increment Process Count. */
	
	proc_curr = allocPcb();			/* use proc_curr temporary */
	/* init state_t */
	proc_curr->p_s.status = _INIT_FIRST_PROC_STATE(0);	/* kernel-mode on, PLT and Interrupts enabled */
	RAMTOP(proc_curr->p_s.reg_sp);						/* SP to RAMTOP (use last RAM fram for its stack) */
	proc_curr->p_s.pc_epc = (memaddr)test;				/* PC to test */

	insertProcQ(&readyQ, proc_curr);
	proc_alive_n++;

	proc_curr = NULL;				/* initialize proc_curr */



	/* 7. Call the Scheduler. */

	Scheduler();

	test();
	
}