#ifndef INITIAL_HELP_H
#define INITIAL_HELP_H

#include "pandos_arch.h"
#include "pandos_types.h"



/*
 * IMPLEMENTATION CONSTANTS
 */

/*	Number of semaphores for devices.
	It's one for each interrupt line, minus 1 as concurrency is not needed,
	plus 3 as there are 2 terminals, each needing 2.
*/
#define N_SEM_DEVICES (N_INTERRUPT_LINES + 2)

/*	Init value for the interval timer */
#define INTERVAL_TIMER_INIT 100



#endif /* INITIAL_HELP_H */