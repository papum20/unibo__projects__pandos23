#include "initial.h"


int main() {

	
	/* 1. Declare the Level 3 global variables. */

	int process_count;
	/*
	integer indicating the number of started, but not
	yet terminated processes.
	*/
	int soft_block_count;
	/*
	A process can be either in the “ready,” “run-
	ning,” or “blocked” (also known as “waiting”) state. This integer
	is the number of started, but not terminated processes that in are
	the “blocked” state due to an I/O or timer request.
	*/
	struct list_head ready_queue;
	/*
	Tail pointer to a queue of pcbs that are in the
	“ready” state.
	*/
	pcb_t current_process;
	/*
	Pointer to the pcb that is in the “running” state,
	i.e. the current executing process.
	*/
	int device_semaphores[];
	/*
	The Nucleus maintains one integer semaphore
	for each external (sub)device in μMPS3, plus one additional semaphore
	to support the Pseudo-clock. [Section 3.6.3]
	Since terminal devices are actually two independent sub-devices,
	the Nucleus maintains two semaphores for each terminal device.
	*/

	/* - Strutture dati gia’ gestite in fase 1 */


	/*
	2. Populate the Processor 0 Pass Up Vector.
	The Pass Up Vector is part
	of the BIOS Data Page, and for Processor 0, is located at 0x0FFF.F900.
	The Pass Up Vector is where the BIOS finds the address of the Nucleus
	functions to pass control to for both TLB-Refill events and all other
	exceptions.
	*/

	/*
	• Set the Nucleus TLB-Refill event handler address to
	xxx->tlb_refll_handler =
	(memaddr) uTLB_RefillHandler;
	where memaddr, in types.h, has been aliased to unsigned int.
	Since address translation is not implemented until the Support
	Level, uTLB_RefillHandler is a place holder function whose code
	is provided. [Section 3.3]
	*/
	/*
	• Set the Stack Pointer for the Nucleus TLB-Refill event handler to
	the top of the Nucleus stack page: 0x2000.1000. Stacks in μMPS3
	grow down.
	*/
	/*
	• Set the Nucleus exception handler address to the address of your
	Level 3 Nucleus function (e.g. foobar) that is to be the entry
	point for exception (and interrupt) handling [Section 3.4]:
	xxx->exception_handler = (memaddr) fooBar;
	*/
	/*
	• Set the Stack pointer for the Nucleus exception handler to the top
	of the Nucleus stack page: 0x2000.1000.
	*/

	/*

	3. Initialize the Level 2 (phase 1 - see Chapter 2) data structures:
	3.1. NUCLEUS INITIALIZATION 23
	initPcbs()
	initASH() initNamespaces()
	4. Initialize all Nucleus maintained variables: Process Count (0), Soft-
	block Count (0), Ready Queue (mkEmptyProcQ()), and Current Process
	(NULL). Since the device semaphores will be used for synchronization,
	as opposed to mutual exclusion, they should all be initialized to zero.
	5. Load the system-wide Interval Timer with 100 milliseconds. [Section
	3.6.3]
	6. Instantiate a single process, place its pcb in the Ready Queue, and
	increment Process Count. A process is instantiated by allocating a pcb
	(i.e. allocPcb()), and initializing the processor state that is part of
	the pcb. In particular this process needs to have interrupts enabled,
	the processor Local Timer enabled, kernel-mode on, the SP set to
	RAMTOP (i.e. use the last RAM frame for its stack), and its PC set
	to the address of test. Furthermore, set the remaining pcb fields as
	follows:
	• Set all the Process Tree fields to NULL.
	• Set the accumulated time field (p_time) to zero.
	• Set the blocking semaphore address (p_semAdd) to NULL.
	• Set the Support Structure pointer (p_supportStruct) to NULL.
	Important Point: When setting up a new processor state one must
	set the previous bits (i.e. IEp & KUp) and not the current bits (i.e.
	IEc & KUc) in the Status register for the desired assignment to take
	effect after the initial LDST loads the processor state. [Section ??-
	pops]
	Test is a supplied function/process that will help you debug your Nu-
	cleus. One can assign a variable (i.e. the PC) the address of a function
	by using
	yyy->p_s.s_pc = (memaddr) test;
	24 CHAPTER 3. PHASE 2 - LEVEL 3: THE NUCLEUS

	For rather technical reasons, whenever one assigns a value to the PC
	one must also assign the same value to the general purpose register t9.
	(a.k.a. reg_t9 as defined in types.h.) [Section ??-pops]
	7. Call the Scheduler.
	Once main() calls the Scheduler its task is complete since control should
	never return to main(). At this point the only mechanism for re-entering
	the Nucleus is through an exception; which includes device interrupts. As
	long as there are processes to run, the processor is executing instructions on
	their behalf and only temporarily enters the Nucleus long enough to handle
	a device interrupt or exception when they occur.
	At boot/reset time the Nucleus is loaded into RAM beginning with the
	second frame of RAM: 0x2000.1000. The first frame of RAM is reserved for
	the Nucleus stack. Furthermore, Processor 0 will be in kernel-mode with
	all interrupts masked, and the processor Local Timer disabled. The PC
	is assigned 0x2000.1000 and the SP, which was initially set to 0x2000.1000
	at boot-time, will now be some value less, due to the activation record for
	main() that now sits on the stack. [Section ??-pops

	test();
	/* L’esecuzione del test e’ corretta se questo arriva 
	al termine senza andare in PANIC. */
}