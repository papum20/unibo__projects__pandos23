#include "scheduler.h"

void Scheduler(){
    
    if(emptyProcQ(&readyQ)){

        if(proc_alive_n==0) HALT();

        else if(proc_alive_n>0 && proc_soft_blocked_n>0){

        STATUS_SET_IE(STATUS_SET_TE(BIT_KERNEL, BIT_DISABLED), BIT_ENABLED);
        
        WAIT();

        }

        else if(proc_alive_n>0 && proc_soft_blocked_n==0) PANIC();

    }

    else{

        proc_curr = removeProcQ(&readyQ);

        setTIMER(_set_time);

        LDST(proc_curr->p_s);
    }
}
