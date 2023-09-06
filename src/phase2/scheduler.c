#include "scheduler.h"

void Scheduler(){
    
    if(proc_alive_n==0 && emptyProcQ(&readyQ)) HALT();

    if(proc_alive_n>0 && proc_soft_blocked_n>0 && emptyProcQ(&readyQ)){

        STATUS_SET_IE(STATUS_SET_TE(BIT_KERNEL, BIT_DISABLED), BIT_ENABLED);
        
        WAIT();
    }
    if(proc_alive_n>0 && proc_soft_blocked_n==0 && emptyProcQ(&readyQ)) PANIC();

    else{

        proc_curr = removeProcQ(&readyQ);

        setTIMER(5);

        LDST(proc_curr->p_s);
    }
}
