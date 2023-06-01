
/****************************************************************************
 *
 * This header file contains utilities for managing memory/registers.
 *
 ****************************************************************************/


#ifndef MEMORY_HELP_H
#define MEMORY_HELP_H

#include "pandos_arch.h"
#include "pandos_types.h"


/*
 * CPU REGISTERS
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
 * BIOS DATA PAGE
 */

/*	Access the Pass Up Vector,
	i.e. the part of the BIOS Data Page, where the BIOS finds the address of the Nucleus functions
	to pass control to for both TLB-Refill events and all other exceptions.
*/
#define PASSUP_VECTOR ((passupvector_t*)BIOS_EXEC_HANDLERS_ADDRS)



#endif /* MEMORY_H */