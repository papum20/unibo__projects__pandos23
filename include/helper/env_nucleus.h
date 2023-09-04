
/****************************************************************************
 *
 * Global variables of the Nucleus (phase2).
 *
 ****************************************************************************/


#ifndef ENV_NUCLEUS_H
#define ENV_NUCLEUS_H


#include "const.h"

#include "pandos_arch.h"
#include "pandos_types.h"



/*
 * ENVIRONMENT VARIABLES
 */

/* number of started, but not yet terminated processes. */
extern int proc_alive_n;


/*	number of started, but not terminated, processes, that are
	in the “blocked” state due to an I/O or timer request.
*/
extern int proc_soft_blocked_n;


/* queue of pcbs that are in the "ready" state. */
extern struct list_head readyQ;


/*	Pointer to the pcb that is in the “running” state,
	i.e. the current executing process.
*/
extern pcb_t *proc_curr;	


/* number of internal devices semaphores, i.e. PLT and Interval Timer */
#define N_INT_IL 2
/* number of terminal devices */
#define N_DEV_TERMINAL 2
/* number of semaphores for each terminal */
#define N_SEM_TERMINAL 2

/*	Number of semaphores for devices.
<<<<<<< HEAD
	It's one for each device on each interrupt line
	(excluding 1 for concurrency, not needed, and terminals),
	plus two for each terminal.
*/
#define N_DEV_SEM ((N_EXT_IL - 1) * N_DEV_PER_IL + 2 * N_SEM_TERMINAL + N_INT_IL)
=======
	It's one for each internal device (timers) and each device on each
	interrupt line (excluding 1 for concurrency, not needed, and terminals),
	plus two for each terminal.
*/
#define N_DEV_SEM ((N_EXT_IL + 1) * N_DEV_PER_IL + N_INT_IL)
>>>>>>> tmp

/*	One integer semaphore for each external (sub)device, plus one
	for the Pseudo-clock, plus four for two, independend terminal
	devices, each needing two semaphores for read and write. 

	Device semaphores are stored in the array in the following order:
	first come the first one of each type, in the same order as interrupt lines
	(cputimer)
*/
extern int dev_sems[N_DEV_SEM];



/*
 * FUNCTIONS AND CONSTANTS
 */

/*
 * Device semaphpores
 */

#define _DEV_NUM_OF_TYPE_MASK ( (N_DEV_PER_IL - 1) << DEV_REG_SIZE )

/* number of external device, from 0 to N_EXT_IL * (N_DEV_PER_IL - 1) + N_DEV_TERMINAL */
#define _DEV_NUM(devAddr) ((uint)devAddr / DEV_REG_SIZE)

/*	offset of the device address from the start of the register
	(only terminals use an offset to indicate either read/write)
*/
#define _DEV_ADDR_OFFSET(devAddr) ((uint)devAddr % DEV_REG_SIZE)

/**	get the index in the array of device semaphores,
	for the external device identified by its address.
	@param devAddr_offset offset of device register address from DEV_REG_START
*/
static inline uint _DEV_SEM_IDX(devregtr devAddr_offset) {
	return _DEV_ADDR_OFFSET(devAddr_offset) ?
		(_DEV_NUM(devAddr_offset) + N_DEV_TERMINAL) : _DEV_NUM(devAddr_offset);
}

/**	
 * get an external device semaphore from the device's register address.
 * @param devAddr device register address
 * @return a semAddr
*/
#define DEV_SEM_FROM_ADDR(devAddr) (dev_sems + _DEV_SEM_IDX(devAddr - DEV_REG_START))
#define SEM_INDEX_LD(line, dev) ( (line - DEV_IL_START) * N_DEV_PER_IL + dev)
/*	Internal devices semaphores (PLT and Interval Timer)
*/
#define DEV_SEM_CPUTIMER (dev_sems + N_DEV_SEM - 2)
#define DEV_SEM_TIMER (dev_sems + N_DEV_SEM - 1)

/**
 * Check if a semaphore key is associated to a device semaphore.
 * @semadd: semaphore key (address)
*/
static inline bool is_DEV_SEM(int *semAdd) {
	return (semAdd >= dev_sems) && (semAdd < dev_sems + N_DEV_SEM);
}

#endif /* ENV_NUCLEUS_H */