
/****************************************************************************
 *
 * Global variables of the Nucleus (phase2).
 *
 ****************************************************************************/


#ifndef ENV_NUCLEUS_H
#define ENV_NUCLEUS_H


#include "pandos_arch.h"
#include "pandos_types.h"



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


/*	Number of semaphores for devices.
	It's one for each interrupt line (excluding terminals),
	minus 1 as concurrency is not needed,
	plus 4 as there are 2 terminals, each needing 2.
*/
#define N_DEV_SEM 10

/* semaphore indexes */
#define SEM_CPUTIMER		0	/* Processor Local Timer (PLT), for scheduling */
#define SEM_TIMER			1	/* Interval Timer, for pseudo-clock ticks */
#define SEM_DISK			2
#define SEM_FLASH			3
#define SEM_ETHERNET		4
#define SEM_PRINTER			5
/* terminals */
#define SEM_TERM0_R			6
#define SEM_TERM0_W			7
#define SEM_TERM1_R			8
#define SEM_TERM1_W			9


/*	One integer semaphore for each external (sub)device, plus one
	for the Pseudo-clock, plus four for two, independend terminal
	devices, each needing two semaphores for read and write. 
*/
extern int dev_sems[N_DEV_SEM];

/* (key of) device semaphore */
#define DEV_SEM(dev) (dev_sems + dev)

/**
 * Check if a semaphore key is associated to a device semaphore.
 * @semadd: semaphore key (address)
*/
static inline bool is_DEV_SEM(int *semAdd) {
	return (semAdd >= dev_sems) && (semAdd < dev_sems + N_DEV_SEM);
}

#endif /* ENV_NUCLEUS_H */