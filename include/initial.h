#ifndef INITIAL_H
#define INITIAL_H



#include "const.h"
#include "memory.h"

#include "pandos_arch.h"
#include "pandos_bios.h"
#include "pandos_types.h"
#include "pandos_types2.h"
#include "types.h"

#include "pcb.h"
#include "ash.h"
#include "ns.h"

#include "exceptions.h"
#include "scheduler.h"



/*
 * MAIN
 */
extern int main();


/*
 * TESTS
 */
/* Test function
*/
extern void test();



#pragma region TODO







	/*
	Pass Up Vector
	Nell’evento di un’eccezione uMPS3 salva lo stato 
	del processore in una specifica locazione di 
	memoria (0x0FFFF000) e carica PC e SP che 
	trova nel Pass Up Vector, una struttura che 
	dovete popolare all’indirizzo 0x0FFFF900.
	Il Pass Up Vector distingue tra TLB-Refill e tutte 
	le altre eccezioni (per distinguere ulteriormente 
	si veda il registro Cause).
	Le eccezioni non possono essere annidate.
	*/


	/*

	The p2test.c code assumes that the TLB Floor Address has been set
	to any value except VM OFF. The value of the TLB Floor Address is a user
	configurable value set via the μMPS3 Machine Configuration Panel. [Chapter
	??]
	The test program reports on its progress by writing messages to TER-
	MINAL0. At the conclusion of the test program, either successful or unsuc-
	cessful, μMPS3 will display a final message and then enter an infinite loop.
	The final message will either be System Halted for successful termination, or
	Kernel Panic for unsuccessful termination
	*/




	/*
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
	effect after the initial LDST loads the processor state.
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

#pragma endregion TODO


#pragma region DONE

	/* INTRODUCTION */

	/*
		- Al contrario di fase 1 il vostro codice ha 
	controllo a partire dal main()

	This module implements main() and exports the Nucleus’s
	global variables. (e.g. process count, device semaphores, etc.
		Every program needs an entry point (i.e. main()). The entry point for
	Pandos performs the Nucleus initialization, which includes:
	*/

	/*
	At boot/reset time the Nucleus is loaded into RAM beginning with the
	second frame of RAM: 0x2000.1000. The first frame of RAM is reserved for
	the Nucleus stack. Furthermore, Processor 0 will be in kernel-mode with
	all interrupts masked, and the processor Local Timer disabled. The PC
	is assigned 0x2000.1000 and the SP, which was initially set to 0x2000.1000
	at boot-time, will now be some value less, due to the activation record for
	main() that now sits on the stack. [Section ??-pops
	*/

	/* STEPS */

	/*
	1. Declare the Level 3 global variables. This should include:
	• Process Count: integer indicating the number of started, but not
	yet terminated processes.
	• Soft-block Count: A process can be either in the “ready,” “run-
	ning,” or “blocked” (also known as “waiting”) state. This integer
	is the number of started, but not terminated processes that in are
	the “blocked” state due to an I/O or timer request.
	• Ready Queue: Tail pointer to a queue of pcbs that are in the
	“ready” state.
	• Current Process: Pointer to the pcb that is in the “running” state,
	i.e. the current executing process.
	
	• Device Semaphores: The Nucleus maintains one integer semaphore
	for each external (sub)device in μMPS3, plus one additional semaphore
	to support the Pseudo-clock. [Section 3.6.3]
	Since terminal devices are actually two independent sub-devices,
	the Nucleus maintains two semaphores for each terminal device.
	[Section ??-pops]

	Strtture dati necessarie
	Variabili globali per:
	- Conteggio dei processi vivi
	- Conteggio dei processi bloccati
	- Coda dei processi “ready”
	- Puntatore al processo correntemente attivo
	- Un semaforo (e.g. una variabile int) per ogni 
	(sub) dispositivo. Non necessariamente tutti 
	questi semafori sono sempre attivi.
	- Strutture dati gia’ gestite in fase 1
	*/

	/*
	2. Populate the Processor 0 Pass Up Vector. The Pass Up Vector is part
	of the BIOS Data Page, and for Processor 0, is located at 0x0FFF.F900.
	[Section ??-pops]
	The Pass Up Vector is where the BIOS finds the address of the Nucleus
	functions to pass control to for both TLB-Refill events and all other
	exceptions. Specifically,
	• Set the Nucleus TLB-Refill event handler address to
	xxx->tlb_refll_handler =
	(memaddr) uTLB_RefillHandler;
	where memaddr, in types.h, has been aliased to unsigned int.
	Since address translation is not implemented until the Support
	Level, uTLB_RefillHandler is a place holder function whose code
	is provided. [Section 3.3] This code will then be replaced when
	the Support Level is implemented.
	• Set the Stack Pointer for the Nucleus TLB-Refill event handler to
	the top of the Nucleus stack page: 0x2000.1000. Stacks in μMPS3
	grow down.
	• Set the Nucleus exception handler address to the address of your
	Level 3 Nucleus function (e.g. foobar) that is to be the entry
	point for exception (and interrupt) handling [Section 3.4]:
	xxx->exception_handler = (memaddr) fooBar;
	• Set the Stack pointer for the Nucleus exception handler to the top
	of the Nucleus stack page: 0x2000.1000.
	*/

	/*
	3. Initialize the Level 2 (phase 1 - see Chapter 2) data structures:
	*/

	/*
	4. Initialize all Nucleus maintained variables: Process Count (0), Soft-
	block Count (0), Ready Queue (mkEmptyProcQ()), and Current Process
	(NULL). Since the device semaphores will be used for synchronization,
	as opposed to mutual exclusion, they should all be initialized to zero.
	*/


	/*
	5. Load the system-wide Interval Timer with 100 milliseconds.
	*/
	/*
	Inizializzazione: dispositivi
	- E’ sufficiente caricare l’Interval Timer con in 
	valore corrispondente a 100 millisecondi, 
	scrivendolo nel registro corrispondente
	- Questo valore dipende dalla frequenza di 
	esecuzione del processore, non puo’ essere una 
	semplice costante.
	*/

	/*
	7. Call the Scheduler.
	Once main() calls the Scheduler its task is complete since control should
	never return to main(). At this point the only mechanism for re-entering
	the Nucleus is through an exception; which includes device interrupts. As
	long as there are processes to run, the processor is executing instructions on
	their behalf and only temporarily enters the Nucleus long enough to handle
	a device interrupt or exception when they occur.
	*/

	/* TEST */

	/*
	Remember to declare test as “external” in your program by including
	the line:
	extern void test();
	
		Il processo di test
	Il processo che si occupa di verificare le 
	funzionalità di test va lanciato alla fine 
	dell’inizializzazione e lasciato operare senza 
	interferenze fino alla fine.
	Sarà sua responsabilità creare nuovi processi 
	usando la system call preposta.
		L’esecuzione del test e’ corretta se questo arriva 
	al termine senza andare in PANIC.
	*/

	/* TIPS */

	/*
	For rather technical reasons, whenever one assigns a value to the PC
	one must also assign the same value to the general purpose register t9.
	*/
	/*
	Since Pandos is intended for uniprocessor environments only,
	interrupt line 0 may safely be ignored
*/

#pragma endregion DONE



#endif /* INITIAL_H */