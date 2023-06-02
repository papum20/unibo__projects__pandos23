#ifndef EXCEPTIONS_HELP_H
#define EXCEPTIONS_HELP_H


#include "pandos_cp0.h"
#include "pandos_types.h"

#include "memory.h"

#include "ash.h"
#include "ns.h"
#include "pcb.h"



/*
 * EXCEPTIONS
 */

/* get cause register setted to exec_code x.
*/
#define __CAUSE_EXCCODE_SETTED(cause, code) (cause & !CAUSE_EXCCODE_MASK) | (code << CAUSE_EXCCODE_BIT)

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
 * copy the new state to pcb p, with pc updated to the next line
 * in order to avoid loop.
 * pcb_t *@p: process whose state is updated.
 */
#define __RETURN_SYSCALL(p) ({					\
	regIncrPC(SAVED_EXCEPTION_STATE);			\
	STATE_CP(*SAVED_EXCEPTION_STATE, &p->p_s);	\
})

/**
 * block the current process on the semaphore.
 * @semaddr: address of the key of the semaphore where to block the process.
 */
static inline void SYSCALL_BLOCK(int *semAdd) {
	update_p_time();
	insertBlocked(semAdd, proc_curr);
	proc_curr = NULL;
}

/**
 * save the return state for current_proc (from a syscall),
 * with return value.
 * int @val: return value
 */
#define RETURN_SYSCALL(val) ({			\
	__RETURN_SYSCALL(proc_curr);		\
	V0(proc_curr) = val;				\
})

/**
 * save the return state for current_proc (from a syscall),
 * without return value.
 */
#define RETURN_SYSCALL_VOID() __RETURN_SYSCALL(proc_curr)

/**
 * save the return state for current_proc (from a syscall),
 * return from syscall (without value) and block the current process.
 * @semaddr: address of the key of the semaphore where to block the process.
 */
static inline void RETURN_SYSCALL_BLOCK(int *semAdd) {
	SYSCALL_BLOCK(semAdd);
	RETURN_SYSCALL_VOID();
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
			SYSCALL_PASSEREN(&a1);					\
			break;									\
		case VERHOGEN:								\
			SYSCALL_VERHOGEN(&a1);					\
			break;									\
		case IOWAIT:								\
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
		case TERMINATE:								\
			SYSCALL_GETPID(a1);						\
			break;									\
		case GET_TOD:								\
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
	if(state != NULL) STATE_CP(*state, &p->p_s);
	p->p_supportStruct = sup;
	(ns != NULL)
	? addNamespace(p, ns)
	: addNamespace(p, getNamespace(p->p_parent, NS_PID));
}


/* terminate all of process' children.
 * process is not NULL.
 */
extern void __terminateTree(pcb_t *p);



#endif /* EXCEPTIONS_HELP_H */