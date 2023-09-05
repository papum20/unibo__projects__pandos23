#ifndef PANDOS23_CONST_H
#define PANDOS23_CONST_H


#include "pandos_arch.h"
#include "pandos_types.h"


/*
 * TYPES
 */

/* Basic types
 */
typedef unsigned int uint;
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
#define SYSCALL_TERMINATE_CURR_PROC 0

/* DOIO (5) */
#define N_ARGS_DEV	4
#define N_ARGS_TERM	2


/*
	INTERRUPTS
*/

#define TERM_WR_LINE 7
#define TERM_RD_LINE 8
#define MULTIPLE_LINE_DEVICES 6
#define ACK_DEVICE 1
#define PLT_RESET 4294967293
#define IT_RESET 100000
#define WRITE_TERMINAL 0
#define READ_TERMINAL 1




#endif /* CONST_H */