#ifndef SCHEDULER_HELP_H
#define SCHEDULER_HELP_H


#include "clocks.h"


/*	Stores the starting TOD clock value for an interval;
	used to calculate the time passed for an interval.
*/
int _interval_start;

int _set_time = 5;


/* Amount of the current schduler time slice used by the current process.
*/
#define TIMESLICE_USED (TODCK - _interval_start)


#endif /* SCHEDULER_HELP_H */