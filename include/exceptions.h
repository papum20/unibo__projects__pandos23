#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


#include "pandos_arch.h"
#include "pandos_const.h"
#include "pandos_cp0.h"
#include "pandos_libumps.h"
#include "pandos_types.h"
#include "types.h"

#include "ash.h"
#include "ns.h"

#include "interrupts.h"
#include "scheduler.h"

#include "const.h"
#include "devices.h"
#include "state.h"



/*
 * EXCEPTIONS
 */

/* dispatcher for all kinds of exceptions. */
extern void Exception_handler();


/* system calls handler */
extern void SYSCALL_handler();


/* pass up exception handling to process' support struct if present,
 * otherwise kill the process.
 */
extern void PassUpOrDie(int excpt_type);


/* program trap handler */
#define Prg_Trap_handler() PassUpOrDie(GENERALEXCEPT)

/* TLB exceptions handler */
#define TLB_handler() PassUpOrDie(PGFAULTEXCEPT)


/*	TLB refill exceptions handler. 
	This function is not defined in this module,
	this it must be imported from somewhere else (test file).
*/
extern void uTLB_RefillHandler();


/*
 * SYSTEM CALLS
 */


/*SYSTEM CALL 1-10*/

/* Create a process, by allocating a pcb;
 * also initializing the fields given as parameters, and the others (including NULL parameters) to 0/NULL.
 * Return -1 on failure.
 */
extern void SYSCALL_CREATEPROCESS (state_t *statep, support_t * supportp, struct nsd_t *ns);

/*	Terminate the process to which the pid is associated, along with its children.
	If pid is 0, the current process is terminated (with its children).
*/
extern void SYSCALL_TERMINATEPROCESS (int pid);

/* perform a P on a semaphore.
*/
extern void SYSCALL_PASSEREN (int *semaddr);

/* perfirm a V on a semaphore.
*/
extern void SYSCALL_VERHOGEN (int *semaddr, int interrupt);

/* Start an IO request.
*/
extern void SYSCALL_DOIO (int *cmdAddr, int *cmdValues);

/*	Return the CPU time used by the calling process.
*/
extern void SYSCALL_GETCPUTIME ();

/* Perform a P on the pseudo-clock semaphore.
 * Thus, the calling process will remain blocked, waiting for a V from the Interval Timer.
 */
extern void SYSCALL_WAITCLOCK ();

/* get a pointer to current process' support struct (or NULL).
*/
extern void SYSCALL_GET_SUPPORT_DATA ();

/* if parent isn't TRUE, return process's pid;
 * else return parent's pid only if it shares process's pid namespace,
 * otherwise 0.
 */
extern void SYSCALL_GETPID (int parent);

/*	Load the `children` array with up to `size` children of the current process
	which are in its same namaspace; return the total number of children.
*/
extern void SYSCALL_GETCHILDREN (int *children, int size);


#endif /* EXCEPTIONS_H */
