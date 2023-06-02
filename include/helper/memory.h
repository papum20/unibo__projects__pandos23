
/****************************************************************************
 *
 * This header file contains utilities for accessing memory/registers addresses.
 *
 ****************************************************************************/


#ifndef MEMORY_HELP_H
#define MEMORY_HELP_H

#include "const.h"

#include "pandos_arch.h"
#include "pandos_bios.h"
#include "pandos_types.h"


/*
 * CPU REGISTERS
 */

/* set the state PC to a new address.
 * ( For rather technical reasons, whenever one assigns a value to the PC
 * one must also assign the same value to the general purpose register t9. )
*/
static inline void regSetPC(state_t *statep, memaddr addr) {
	statep->pc_epc = addr;
	statep->reg_t9 = addr;
}

/* set PC to next instruction, i.e. increment by one address.
*/
static inline void regIncrPC(state_t *statep) {
	statep->pc_epc += WORDLEN;
}

/* STATE_T
*/

/* Status bits values
*/
#define BIT_KERNEL 0
#define BIT_USER 1

/* check if the current saved exception state is in User Mode.
*/
#define IS_UM ( (SAVED_EXCEPTION_STATE->status & STATUS_KUp) == BIT_USER )

/* copy a state from src to dst.
*/
static inline void STATE_CP(state_t src, state_t *dst) {
	dst->entry_hi	= src.entry_hi;
	dst->cause		= src.cause;
	dst->status		= src.status;
	dst->pc_epc		= src.pc_epc;
	dst->hi			= src.hi;
	dst->lo			= src.lo;

	for(int i=0; i < STATE_GPR_LEN; i++)
		dst->gpr[i] = src.gpr[i];
}


/*
 * BIOS DATA PAGE
 */

/* Access the Pass Up Vector,
 * i.e. the part of the BIOS Data Page, where the BIOS finds the address of the Nucleus functions
 * to pass control to for both TLB-Refill events and all other exceptions.
*/
#define PASSUP_VECTOR ((passupvector_t*)BIOS_EXEC_HANDLERS_ADDRS)

/* Access the saved exception state,
 * i.e. the location where a process' state is saved when an exception happens.
 */
#define SAVED_EXCEPTION_STATE  ((state_t *)BIOS_DATA_PAGE_BASE)


/*
 * DEVICES
 */

/* get the device interrupt line from its address */
#define DEV_IL(addr) ( (addr - DEV_REG_START) / (DEV_REG_SIZE * N_DEV_PER_IL) + DEV_IL_START )

/* copy the DOIO arguments (cmdValues) to the device register (cmdAddr)
*/
static inline void dev_setArgs(int *cmdAddr, int *cmdValues) {

	/* get the right number of arguments, depending on the device type*/
	int argc = (DEV_IL((memaddr)cmdAddr) == IL_TERMINAL) ? N_ARGS_TERM : N_ARGS_DEV;
	/* copy register fields */
	for( ; argc > 0; argc--)
		*(cmdAddr + argc * WORDLEN) = *(cmdValues + argc * WORDLEN);
}



#endif /* MEMORY_H */