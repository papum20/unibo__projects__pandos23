#include "scheduler.h"


/*	Stores the starting TOD clock value for an interval;
	used to calculate the time passed for an interval.
*/
int _interval_start;


void Scheduler(){

	/* Scheduler is called in kernel mode */
    
    if(proc_alive_n==0)
		HALT();

    else if(proc_alive_n>0 && proc_soft_blocked_n>0 && emptyProcQ(&readyQ)){

		setSTATUS(
			STATUS_SET_IEc_ALL(
				STATUS_SET_TE(
					getSTATUS(), BIT_DISABLED
				),
				BIT_ENABLED
			)
		);
        
        WAIT();

    }

    else if(proc_alive_n>0 && proc_soft_blocked_n==0 && emptyProcQ(&readyQ)) 
        PANIC();

    else{

        proc_curr = removeProcQ(&readyQ);

        setTIMER(TIMESLICE);
		_interval_start = 0;

        LDST(&proc_curr->p_s);
    }
}
