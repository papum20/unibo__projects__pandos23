#ifndef EXCEPTIONS_HELP_H
#define EXCEPTIONS_HELP_H


#include "pandos_const.h"
#include "pandos_cp0.h"
#include "pandos_types.h"

#include "memory.h"
#include "state.h"

#include "ash.h"
#include "ns.h"
#include "pcb.h"
#include "scheduler.h"

#include "env_nucleus.h"
#include "pcb_help.h"
#include "scheduler_help.h"



/*
 * EXCEPTIONS
 */

/* get cause register setted to exec_code x.
*/
#define __CAUSE_EXCCODE_SETTED(cause, code) (cause & ~CAUSE_EXCCODE_MASK) | (code << CAUSE_EXCCODE_BIT_START)

/* set cause register to exec_code code for the Saved Exception State.
*/
#define CAUSE_SET_EXCCODE(code) SAVED_EXCEPTION_STATE->cause = __CAUSE_EXCCODE_SETTED(SAVED_EXCEPTION_STATE->cause, code)


/* check if an exception code is associated to a TLB exception.
*/
#define IS_TLB_EXCEPTION(code) (code >= EXC_MOD && code <= EXC_TLBS)

/* check if an exception code is associated to a Trap exception.
*/
#define IS_TRAP_EXCEPTION(code) (code >= EXC_ADEL && code <= EXC_DBE) || (code >= EXC_BP && code <= EXC_OV)



/*
 * SYSTEM CALLS
 */

/* return value for error */
#define ERR -1


/* check if syscall code is valid (i.e. first parameter, a0)
*/
#define IS_SYSCALL_CODE_VALID(code) (code >= CREATEPROCESS && code <= GET_TOD)


/**
 * Common operations for returning from a syscall.
 * Increment the PC.
 */
#define __RETURN_SYSCALL() ({					\
	regIncrPC(SAVED_EXCEPTION_STATE);			\
})

/**
 * Block the current process on the semaphore, and perform some operations,
 * like storing the processor state and updating processor time and auxiliary variables.
 * @semaddr: address of the key of the semaphore where to block the process.
 */
static inline void SYSCALL_BLOCK(int *semAdd) {
	STATE_CP(*SAVED_EXCEPTION_STATE, &proc_curr->p_s);
	PROC_TIME_UPDATE(proc_curr);
	insertBlocked(semAdd, proc_curr);
	proc_curr = NULL;
	if(is_DEV_SEM(semAdd)) proc_soft_blocked_n++;
}

/**
 * save the return state for current_proc (from a syscall),
 * with return value.
 * Return the control, reloading the processor state.
 * int @val: return value
 */
#define RETURN_SYSCALL(val) ({				\
	__RETURN_SYSCALL();						\
	SAVED_EXCEPTION_STATE->reg_v0 = val;	\
	LDST(SAVED_EXCEPTION_STATE);			\
})

/**
 * save the return state for current_proc (from a syscall),
 * without return value.
 * Return the control, reloading the processor state.
 */
#define RETURN_SYSCALL_VOID() ({	\
	__RETURN_SYSCALL();	\
	LDST(SAVED_EXCEPTION_STATE);	\
})

/**
 * save the return state for current_proc (from a syscall),
 * return from syscall (without value) and block the current process.
 * @semaddr: address of the key of the semaphore where to block the process.
 */
static inline void RETURN_SYSCALL_BLOCK(int *semAdd) {
	__RETURN_SYSCALL();
	SYSCALL_BLOCK(semAdd);
	Scheduler();
}


/* syscall dispatcher,
 * i.e. dispatches to syscall sub-functions.
 */
/* syscall dispatcher, taking a variable as a0
*/
#define _SYSCALL(a0, a1, a2, a3)					\
	switch(a0){										\
		case CREATEPROCESS:							\
			SYSCALL_CREATEPROCESS((state_t *)a1, (support_t *) a2, (struct nsd_t *) a3);	\
			break;									\
		case TERMPROCESS:							\
			SYSCALL_TERMINATEPROCESS (a1);			\
			break;									\
		case PASSEREN:								\
			SYSCALL_PASSEREN((int *)a1);			\
			break;									\
		case VERHOGEN:								\
			SYSCALL_VERHOGEN((int *)a1, FALSE);		\
			break;									\
		case DOIO:									\
			SYSCALL_DOIO((int *)a1,(int *)a2);		\
			break;									\
		case GETTIME:								\
			SYSCALL_GETCPUTIME();					\
			break;									\
		case CLOCKWAIT:								\
			SYSCALL_WAITCLOCK();					\
			break;									\
		case GETSUPPORTPTR:							\
			SYSCALL_GET_SUPPORT_DATA();				\
			break;									\
		case GETPROCESSID:							\
			SYSCALL_GETPID(a1);						\
			break;									\
		case GETCHILDREN:							\
			SYSCALL_GETCHILDREN((int *)a1, a2);		\
		}	

/* syscall dispatcher, taking a syscall identifier constant as a0 (e.g. DOIO)
*/
#define SYSCALL(a0, a1, a2, a3)		\
	int _a0 = a0;					\
	_SYSCALL(_a0, a1, a2, a3)


/**
 * init proc's pcb fields (for create_process syscall).
 * @p: proc to init
 * @prnt: new parent
 * @state: new state
 * @sup: new support struct
 * @ns: new pid namespace. set to parent's pid ns if it's null.
*/
static inline void __init_createProc(pcb_t *p, pcb_t *prnt, state_t *state, support_t *sup, nsd_t *ns) {
	insertChild(prnt, p);
	p->p_parent = prnt;
	if(state != NULL) STATE_CP(*state, &p->p_s);
	p->p_supportStruct = sup;
	if (ns != NULL)
		addNamespace(p, ns);
	else if(getNamespace(p->p_parent, NS_PID) != NULL)
		addNamespace(p, getNamespace(p->p_parent, NS_PID));
}


/*	unblock a `pcb_t *`, removing the first element blocked on 
	the semaphore with key `int *semAdd` and inserting it in the `readyQ`.
 */
#define unblockPcb(semAdd) insertProcQ(&readyQ, removeBlocked(semAdd))


/* terminate all of process' children, with `p` != NULL.
*/
extern void tmp__terminateDescendants(pcb_t *p);



#endif /* EXCEPTIONS_HELP_H */