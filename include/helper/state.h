
/****************************************************************************
 *
 * This header file contains utilities for managing processor states (state_t).
 *
 ****************************************************************************/


#ifndef STATE_H
#define STATE_H


#include "memory.h"

#include "pandos_arch.h"
#include "pandos_bios.h"
#include "pandos_cp0.h"
#include "pandos_types.h"



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
 * STATUS
 */

/* Masks
*/
#define STATUS_IEp_MASK (!STATUS_IEp)
#define STATUS_KUp_MASK (!STATUS_KUp)
#define STATUS_TE_MASK (!STATUS_TE)		/* PLT */

/* Kernel/User mode
*/

#define BIT_KERNEL 0
#define BIT_USER 1

#define BIT_DISABLED 0
#define BIT_ENABLED 1


/* Utilities */

/* check if the current saved exception state is in User Mode.
*/
#define IS_UM ( (SAVED_EXCEPTION_STATE->status & STATUS_KUp) == BIT_USER )


/* Macros to set status bits.
*/

/**
 * Return the given status with the `mode` bit (0/1)
 * applied to the specified `bit` with the given `mask`.
 * 
 * As specified in the documentation, cpu state must be set using previous fields
 * (e.g. IEp).
 */
#define _STATUS_SET(status, mode, mask, bit) ( (status & mask) | (mode << bit) )

/* Return the given status with the `mode` bit (kernel/user) applied.
*/
#define STATUS_SET_KU(status, mode) _STATUS_SET(status, mode, STATUS_KUp_MASK, STATUS_KUp_BIT)

/* Return the given status with the `mode` bit (interrupts enabled/disabled) applied.
*/
#define STATUS_SET_IE(status, mode) _STATUS_SET(status, mode, STATUS_IEp_MASK, STATUS_IEp_BIT)

/* Return the given status with the `mode` bit (PLT enabled/disabled) applied.
*/
#define STATUS_SET_TE(status, mode) _STATUS_SET(status, mode, STATUS_TE_MASK, STATUS_TE_BIT)



/* Cause code */
#define CAUSE_IP_START 8	/* starting bit */



#endif /* STATE_H */