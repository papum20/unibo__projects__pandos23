
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

/*	Number of semaphores for devices.
	It's one for each device on each interrupt line
	(excluding 1 for concurrency, not needed, and terminals),
	plus two for each terminal.
*/
#define N_DEV_SEM (N_EXT_IL * N_DEV_PER_IL + N_DEV_PER_IL + N_INT_IL)

/*	One integer semaphore for each external (sub)device, plus one
	for the Pseudo-clock, plus four for two, independend terminal
	devices, each needing two semaphores for read and write. 
*/
extern int dev_sems[N_DEV_SEM];



/*
 * FUNCTIONS AND CONSTANTS
 */

/*
 * Device semaphpores
 */

#define _DEV_NUM_REL_MASK ( (N_DEV_PER_IL - 1) << DEV_REG_SIZE )

/* type of device, corresponding to an Interrupt Line */
#define _DEV_NUM(devAddr) ((uint)devAddr / DEV_REG_SIZE)

/* number of device, between those of its type */
#define _DEV_NUM_REL(devAddr) ((uint)devAddr & _DEV_NUM_REL_MASK)

/* offset of the device address from the start of the register */
#define _DEV_ADDR_OFFSET(devAddr) ((uint)devAddr % DEV_REG_SIZE)

/**	get the index for the array of device semaphores,
	relative to the device identified by its address.
*/
static inline DEV_SEM_IDX(int *devAddr) {
	return _DEV_ADDR_OFFSET(devAddr) ?
		_DEV_NUM(devAddr) : (_DEV_NUM(devAddr) + _DEV_NUM_REL(devAddr));
}

/**	get a device semaphore (int *) from the device's register address.
*/
#define DEV_SEM_FROM_ADDR(devAddr) (dev_sems + DEV_SEM_IDX(devAddr))

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