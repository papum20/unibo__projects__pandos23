#ifndef SCHEDULER_HELP_H
#define SCHEDULER_HELP_H


#include "devices.h"


/* Scheduler timeslice in microseconds */
#define TIMESLICE 5000


/*	Stores the starting PLT clock value for an interval;
	used to calculate the time passed for an interval.
*/
extern int _interval_start;


/*	Amount of the current schduler time slice used by the current process.
	Note that PLT counts down.
*/
#define TIMESLICE_USED (_interval_start - getTIMER())

/* CPU time used by a process */
#define CPU_TIME_USED(proc) (proc->p_time + TIMESLICE_USED)

/*	update proc's accumulated processor time */
#define PROC_TIME_UPDATE(proc) (proc->p_time += TIMESLICE_USED)




#endif /* SCHEDULER_HELP_H */