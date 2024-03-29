#include "initial.h"
#include "helper/initial_help.h"



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
int dev_sems[SEM_DEVICES_N];




int main() {




	/* 2. Populate the (Processor 0) Pass Up Vector. */
	
	/*	Set the Nucleus exception and TLB-Refill event handlers,
		and their SP to the top of the Nucleus stack page.
	*/
	PASSUP_VECTOR->tlb_refill_handler	= (memaddr) uTLB_RefillHandler;
	PASSUP_VECTOR->tlb_refill_stackPtr	= (memaddr) KERNELSTACK;
	PASSUP_VECTOR->exception_handler	= (memaddr) exceptionHandler;
	PASSUP_VECTOR->exception_stackPtr	= (memaddr) KERNELSTACK;



	/* 3. Initialize the Level 2 (phase 1 - see Chapter 2) data structures */

	initPcbs();
	initASH();
	initNamespaces();



	/* 4. Initialize all Nucleus maintained variables */

	proc_alive_n			= 0;
	proc_soft_blocked_n	= 0;
	mkEmptyProcQ (		&readyQ );
	proc_curr		= NULL;
	
	/* Semaphores set to 0, as they will be used for synchronization. */
	for(int i = 0; i < SEM_DEVICES_N; i++)
		dev_sems[i] = SEM_VALUE_SYNC;



	/* 5. Load the system-wide Interval Timer. */
	LDIT(INTERVAL_TIMER_INIT);



	/* 6. Instantiate a single process, place its pcb in the Ready Queue, and
	increment Process Count. */
	
	insertProcQ(&readyQ, allocPcb());
	proc_alive_n++;

	/* NOTES:
	1.	to do when the new pcb_t will be released (containing the p_supportStruct field):
		Set the Support Structure pointer (p_supportStruct) to NULL.
		In particular this process needs to have interrupts enabled
		the processor Local Timer enabled
		kernel-mode on
		the SP set to RAMTOP (i.e. use the last RAM frame for its stack)
		and its PC set to the address of test
	*/


	/*
	Inizializzazione: scheduler
	- Allocare un processo (pcb_t) in kernel mode, 
	con interrupt abilitati, stack pointer a RAMTOP e 
	program counter che punta alla funzione test() 
	(fornita da noi).
	- Inserire questo processo nella Ready Queue.
	- invocare lo scheduler.

	*/

	/* 7. Call the Scheduler. */

	scheduler();


	/* L’esecuzione del test e’ corretta se questo arriva 
	al termine senza andare in PANIC. */
	test();
}