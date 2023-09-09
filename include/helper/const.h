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
	if(*semAdd > 1) (*semAdd)--;
	else if(*semAdd < 0) (*semAdd)++;
}


/*
 * SYSCALLS
 */

/* the four arguments for SYSCALL(), from a processor state address
*/
#define A0(state) state->reg_a0
#define A1(state) state->reg_a1
#define A2(state) state->reg_a2
#define A3(state) state->reg_a3
#define V0(state) state.reg_v0

/* parameters
*/

/* TERMPROCESS (2) */
#define SYSCALL_TERMINATE_CURR_PROC 0

/* DOIO (5) */
#define N_ARGS_DEV	4
#define N_ARGS_TERM	2



#endif /* CONST_H */