#ifndef PANDOS23_CONST_H
#define PANDOS23_CONST_H


#include "pandos_types.h"


/*
 * TYPES
 */

/* Basic types
 */
typedef unsigned int * uint_PTR;

/*	alias of static, for disambiguating its use to declare
	"private" functions, from its use as constant.
*/
#define HIDDEN static


/*
 * SEMAPHORES
 */

/* semaphores (init) values */
#define SEM_VALUE_SYNC 0
#define SEM_VALUE_MUTEX 1

/**
 * Adjust a binary semaphore key to keep the invariant
 * when a process is removed from it.
 * @semadd: semaphores key (address)
 */
static inline void SEM_ADJUST(int *semAdd) {
	(*semAdd > 1) ? (*semAdd)-- : (*semAdd)++;
}


/*
 * SYSCALLS
 */

/* the four arguments for SYSCALL()
*/
#define A0(proc) proc->p_s.reg_a0
#define A1(proc) proc->p_s.reg_a1
#define A2(proc) proc->p_s.reg_a2
#define A3(proc) proc->p_s.reg_a3
#define V0(proc) proc->p_s.reg_v0

/* parameters
*/
/* TERMPROCESS (2) */
#define SYSCALL_TERMINATE_CURR_PROC


/*
 * STATE_T
 */

/* check if a state is in User Mode.
*/
#define IS_UM(state) (SAVED_EXCEPTIONS_STATE->status == BIT_USER)

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



#endif /* CONST_H */