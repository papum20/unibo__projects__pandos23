#ifndef INITIAL_HELP_H
#define INITIAL_HELP_H

#include "pandos_arch.h"



/*
 * IMPLEMENTATION CONSTANTS
 */

/*	Number of semaphores for devices.
	It's one for each interrupt line, minus 1 as concurrency is not needed,
	plus 3 as there are 2 terminals, each needing 2.
*/
#define N_SEM_DEVICES (N_INTERRUPT_LINES + 2)


/*
 * MEMORY MANAGEMENT - CPU REGISTERS
 */

/*	set the state PC to a new address.
	( For rather technical reasons, whenever one assigns a value to the PC
	one must also assign the same value to the general purpose register t9. )
*/
static inline void regSetPC(state_t *statep, memaddr addr) {
	statep->pc_epc = addr;
	statep->reg_t9 = addr;
}

/*
 * MEMORY MANAGEMENT - CPU BIOS DATA PAGE
 */

/*	Access the Pass Up Vector,
	i.e. the part of the BIOS Data Page, where the BIOS finds the address of the Nucleus functions
	to pass control to for both TLB-Refill events and all other exceptions.
*/
#define PASSUP_VECTOR = ((passupvector_t*) BIOS_EXEC_HANDLERS_ADDRS)



#endif /* INITIAL_HELP_H */