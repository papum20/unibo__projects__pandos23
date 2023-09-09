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

        /*disable the PLT and enable the interrupt*/
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

        /*Remove the pcb from the head of the Ready Queue and store it in the current process*/
        proc_curr = removeProcQ(&readyQ);

        /*set the PLT*/
        setTIMER(TIMESLICE);
		_interval_start = 0;

        /*load the processor state*/
        LDST(&proc_curr->p_s);
    }
}
