#ifndef INITIAL_HELP_H
#define INITIAL_HELP_H

#include "pandos_arch.h"



/* Test function
*/
extern void test();



/*
 * EXCEPTIONS
 */

/* TLB_Refill exception handler - provided.
*/
extern void uTLB_RefillHandler();



/*
 * IMPLEMENTATION CONSTANTS
 */

/* Number of semaphores for devices */
#define N_SEM_DEVICES N_INTERRUPT_LINES


/*
 * MEMORY MANAGEMENT
 */

/* set the state PC to a new address.
( For rather technical reasons, whenever one assigns a value to the PC
one must also assign the same value to the general purpose register t9. )
*/
static inline void regSetPC(state_t *statep, memaddr addr) {
	statep->pc_epc = addr;
	statep->reg_t9 = addr;
}



#endif /* INITIAL_HELP_H */