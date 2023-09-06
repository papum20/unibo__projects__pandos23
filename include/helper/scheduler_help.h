#ifndef SCHEDULER_HELP_H
#define SCHEDULER_HELP_H


#include "devices.h"


/*	Stores the starting TOD clock value for an interval;
	used to calculate the time passed for an interval.
*/
int _interval_start;

int _set_time = 5;


/* Amount of the current schduler time slice used by the current process.
*/
#define TIMESLICE_USED (TODCK - _interval_start)

/* CPU time used by a process */
#define CPU_TIME_USED(proc) (proc->p_time + TIMESLICE_USED)

/*	update proc's accumulated processor time */
#define PROC_TIME_UPDATE(proc) (proc->p_time += TIMESLICE_USED)




#endif /* SCHEDULER_HELP_H */