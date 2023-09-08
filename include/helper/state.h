
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
#define STATUS_IEc_MASK (~STATUS_IEc)
#define STATUS_IEp_MASK (~STATUS_IEp)
#define STATUS_KUp_MASK (~STATUS_KUp)
#define STATUS_TE_MASK	(~STATUS_TE)		/* PLT */
#define STATUS_CU0_MASK	(~STATUS_CU0)
#define STATUS_IEc_ALL_MASK (~(STATUS_IEc | STATUS_IM_MASK) )
#define STATUS_IEp_ALL_MASK (~(STATUS_IEp | STATUS_IM_MASK) )

/* status bits values
*/
#define BIT_DISABLED 0
#define BIT_ENABLED 1

/* Kernel/User mode
*/

#define BIT_KERNEL 0
#define BIT_USER 1


/* Utilities */

/* check if the current saved exception state is in User Mode.
*/
#define IS_UM ( ( (SAVED_EXCEPTION_STATE->status & STATUS_KUp) >> STATUS_KUp_BIT) == BIT_USER )


/* Macros to set status bits.
*/

/**
 * Return the given status with the `mode` bit (0/1)
 * applied to the bits starting at the specified `shift` with the given `mask`
 * (where there are 0 in the fields to modify).
 * 
 * As specified in the documentation, cpu state must be set using previous fields
 * (e.g. IEp).
 */
#define _STATUS_SET(status, mode, mask, shift) ( (status & mask) | (mode << shift) )

/* Return the given status with the `mode` bit (kernel/user) applied.
*/
#define STATUS_SET_KUp(status, mode)	_STATUS_SET(status, mode, STATUS_KUp_MASK, STATUS_KUp_BIT)

/* Return the given status with the `mode` bit (interrupts enabled/disabled) applied (prev).
*/
#define STATUS_SET_IEp(status, mode)	_STATUS_SET(status, mode, STATUS_IEp_MASK, STATUS_IEp_BIT)

/* Return the given status with the `mode` bit (interrupts enabled/disabled) applied (current).
*/
#define STATUS_SET_IEc(status, mode)	_STATUS_SET(status, mode, STATUS_IEc_MASK, STATUS_IEc_BIT)

/* Return the given status with the `mode` bit (PLT enabled/disabled) applied.
*/
#define STATUS_SET_TE(status, mode)		_STATUS_SET(status, mode, STATUS_TE_MASK, STATUS_TE_BIT)

/* Return the given status with the `mode` bit (PLT enabled/disabled) applied.
*/
#define STATUS_SET_CU0(status, mode)	_STATUS_SET(status, mode, STATUS_CU0_MASK, STATUS_CU0_BIT)

/* Return the given status with all interrupt enabled, i.e. the bit `IEp` and all the masks on (current).
*/
#define STATUS_SET_IEc_ALL(status, mode)	_STATUS_SET(status, mode * (~STATUS_IEc_ALL_MASK), STATUS_IEc_ALL_MASK, 0)

/* Return the given status with all interrupt enabled, i.e. the bit `IEp` and all the masks on (prev).
*/
#define STATUS_SET_IEp_ALL(status, mode)	_STATUS_SET(status, mode * (~STATUS_IEp_ALL_MASK), STATUS_IEp_ALL_MASK, 0)



/* Cause code */
#define CAUSE_IP_START 8	/* starting bit */



#endif /* STATE_H */