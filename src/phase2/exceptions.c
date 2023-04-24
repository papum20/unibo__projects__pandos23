/*
exceptions.c
This module implements the TLB, Program Trap, and
SYSCALL exception handlers. Furthermore, this module will contain
the provided skeleton TLB-Refill event handler (e.g. uTLB_RefillHandler).
*/

#include "exceptions.h"

void uTLB_RefillHandler() {

    setENTRYHI(0x80000000);
    setENTRYLO(0x00000000);
    TLBWR();

    LDST((state_t *)0x0FFFF000);
	
}

extern void SYSCALL_handler(){
	pcb_t * curr_process = runningProc();
	p_u_int a1 = &curr_process->p_s.reg_a1;
	p_u_int a2 = &curr_process->p_s.reg_a2;
	p_u_int a3 = &curr_process->p_s.reg_a3;
	unsigned int result;
	if(curr_process->p_s.status==1 && ((curr_process->p_s.reg_a0>=0) && (curr_process->p_s.reg_a0<=10))){/*significa che sei in user mode e non va bene, da chiedere al prof*/
	/*
	setting
	Cause.ExcCode in the stored exception state to RI (Reserved Instruction),
	and calling one’s Program Trap exception handler.
	*/
	}
	switch(curr_process->p_s.reg_a0){
		case CREATEPROCESS:
			result = SYSCALL_CREATEPROCESS((state_t *)*a1, (support_t *) *a2, (struct nsd_t *) *a3);
			break;
		case TERMPROCESS:
			SYSCALL_TERMINATEPROCESS (*a1);
			break;
		case PASSEREN:
			SYSCALL_PASSEREN((int *)a1);
			break;
		case VERHOGEN:
			SYSCALL_VERHOGEN((int *)a1);
			break;
		case IOWAIT:
			result = SYSCALL_DOIO((int *)a1,(int *)a2);
			break;
		case GETTIME:
			result = SYSCALL_GETCPUTIME();
			break;
		case CLOCKWAIT:
			SYSCALL_WAITCLOCK();
			break;
		case GETSUPPORTPTR:
			result = (unsigned int)SYSCALL_GETSUPPORTPTR();/*qua come faccio?*/
			break;
		case TERMINATE:
			result = SYSCALL_GETPID(*a1);
			break;
		case GET_TOD:
			result = SYSCALL_GETCHILDREN((int *)a1, *a2);
			break;

	}
	curr_process->p_s.reg_v0 = result;
}
