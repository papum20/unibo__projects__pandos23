
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
/* number of semaphores for each terminal */
#define N_SEM_TERMINAL 2

/*	Number of semaphores for devices.
	It's one for each internal device (timers) and each device on each
	interrupt line (excluding 1 for concurrency, not needed, and terminals),
	plus two for each terminal.
*/
#define N_DEV_SEM ((N_EXT_IL + 1) * N_DEV_PER_IL + N_INT_IL)

/*	One integer semaphore for each external device, two for each terminal,
	plus one for each internal clock.

	Device semaphores are stored in the array in the following order:
	first come the external devices, in the same order as their interrupt lines;
	then, assuming terminals are the last devices, come other N_DEV_PER_IL semaphores
	for each terminal, representing the receive ends, which only use the bytes 3-4 of the register
	(while the first N_DEV_PER_IL represent their respective transmit ends, using bytes 1-2);
	lastly come cputimer and interval timer.
*/
extern int dev_sems[N_DEV_SEM];



/*
 * FUNCTIONS AND CONSTANTS
 */

/*
 * Device semaphpores
 */

#define _EXT_DEV_NUM_OF_TYPE_MASK ( (N_DEV_PER_IL - 1) << DEV_REG_SIZE )

/* number of external device, from 0 to N_EXT_IL * N_DEV_PER_IL */
#define _EXT_DEV_NUM(devAddr_offset) ((uint)devAddr_offset / DEV_REG_SIZE)

/*	offset of the device address from the start of the register
	(only terminals use an offset to indicate either read/write)
*/
#define _EXT_DEV_ADDR_OFFSET(devAddr) ((uint)devAddr % DEV_REG_SIZE)

/*	Internal devices semaphores (PLT and Interval Timer)
*/
#define DEV_SEM_CPUTIMER	(dev_sems + N_DEV_SEM - N_EXT_IL + IL_CPUTIMER)
#define DEV_SEM_TIMER		(dev_sems + N_DEV_SEM - N_EXT_IL + IL_TIMER)

/**	get the index in the array of device semaphores,
 *	for the external device identified by its address.
 *	@param devAddr_offset offset of device register address from DEV_REG_START
 */
static inline uint _EXT_DEV_SEM_IDX(devregtr devAddr_offset) {
	return _EXT_DEV_ADDR_OFFSET(devAddr_offset) ?
		(_EXT_DEV_NUM(devAddr_offset) + N_DEV_PER_IL) : _EXT_DEV_NUM(devAddr_offset);
}

/**	
 * Get an external device semaphore from the device's register address.
 * For terminals, the command values are 2 bytes long, so `devAddr` can start 
 * at its device register address + 2.
 * @param devAddr device register address, of type `devrgtr`
 * @return a semAddr
 */
#define EXT_DEV_SEM_FROM_REGADDR(devAddr) (dev_sems + _EXT_DEV_SEM_IDX(devAddr - DEV_REG_START))

/**	
 * get a device semaphore from the device's interrupt line and device number.
 * @param line interrupt line
 * @param dev device number
 * @return a semAddr
 */
#define DEV_SEM_FROM_LINEDEV(line, dev) (dev_sems +										\
	( (line < N_EXT_IL) ?																\
		N_DEV_SEM - (N_EXT_IL - 1 - dev) :					/* case internal device */	\
		_EXT_DEV_SEM_IDX(DEV_REG_OFFSET(line, dev))			/* case external device */	\
	))

/**
 * Check if a semaphore key is associated to a device semaphore.
 * @semadd: semaphore key (address)
 */
static inline bool is_DEV_SEM(int *semAdd) {
	return (semAdd >= dev_sems) && (semAdd < dev_sems + N_DEV_SEM);
}



#endif /* ENV_NUCLEUS_H */