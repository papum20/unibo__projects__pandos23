/*
exceptions.c
This module implements the TLB, Program Trap, and
SYSCALL exception handlers. Furthermore, this module will contain
the provided skeleton TLB-Refill event handler (e.g. uTLB_RefillHandler).
*/


#include "initial.h"
#include "pandos_cp0.h"
#include "pandos_types2.h"
#include "pandos_const.h"
#include "interrupts.h"

void cp_state(state_t* src_state, state_t* dst_state){
	dst_state->entry_hi = src_state->entry_hi;
	dst_state->cause = src_state->cause;
	dst_state->status = src_state->status;
	dst_state->pc_epc = src_state->pc_epc;
	dst_state->hi = src_state->hi;
	dst_state->lo = src_state->lo;
	for(int i=0;i<29;i++){dst_state->gpr[i]=src_state->gpr[i];}
}

void uTLB_RefillHandler() {

    setENTRYHI(0x80000000);
    setENTRYLO(0x00000000);
    TLBWR();

    LDST((state_t *)0x0FFFF000);
	
}
void PassUpOrDie(int index){
	support_t * curr_support = SYSCALL_GETSUPPORTPTR();
	if(curr_support==NULL){
		SYSCALL_TERMINATEPROCESS(TERMINATE_CURR_PROCESS);
		return;
	}
	/*da continuare caso se support non è null*/

}
void Exception_handler(){
	uint_PTR exeCode = &current_proc->p_s.cause;
	*exeCode = CAUSE_GET_EXCCODE(*exeCode);
	if(*exeCode==EXC_INT){
		Interrupt_handler();
	}
	else if(TLB_EXCEPTION(*exeCode)){
		TLB_handler();
	}
	else if((*exeCode>=EXC_ADEL && *exeCode<=EXC_DBE) || (*exeCode>=EXC_BP && *exeCode<=EXC_OV))//Program Trap
	{
		Prg_Trap_handler();
	}
	else if(*exeCode == EXC_SYS){
		SYSCALL_handler();
	}

}

void SYSCALL_handler(){
	uint_PTR a1 = &current_proc->p_s.reg_a1;
	uint_PTR a2 = &current_proc->p_s.reg_a2;
	uint_PTR a3 = &current_proc->p_s.reg_a3;
	memaddr result;
	if((current_proc->p_s.status==BIT_USER) && ((current_proc->p_s.status>=CREATEPROCESS) && (current_proc->p_s.status<=GET_TOD))){/*significa che sei in user mode e non va bene, da chiedere al prof*/
		uint_PTR exeCode = &current_proc->p_s.cause;
		*exeCode = CAUSE_GET_EXCCODE(*exeCode); 
		*exeCode = EXC_RI;     /*setto il registro exeCode in RI e poi chiamo il program Trapp exception handler*/
	/*
		ProgramTrapExceptionHandler();
	*/
	}
	switch(current_proc->p_s.reg_a0){
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
		default:  /*system call con a0 >= 11*/
			PassUpOrDie(GENERALEXCEPT);
			break;

	}
	current_proc->p_s.reg_v0 = result;
}
void Prg_Trap_handler(){
	PassUpOrDie(GENERALEXCEPT);
}
void TLB_handler(){
	PassUpOrDie(PGFAULTEXCEPT);
}
