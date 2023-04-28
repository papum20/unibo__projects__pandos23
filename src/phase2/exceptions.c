/*
exceptions.c
This module implements the TLB, Program Trap, and
SYSCALL exception handlers. Furthermore, this module will contain
the provided skeleton TLB-Refill event handler (e.g. uTLB_RefillHandler).
*/


#include "exceptions.h"
/*questo lo metto extern come mi ha suggerito Daniele così si collega al suo puntatore del current_proc del suo file init, per 
questo qui non devo mettergli valore ma solo dichiararlo o accederci */
extern pcb_t * current_proc;



void uTLB_RefillHandler() {

    setENTRYHI(0x80000000);
    setENTRYLO(0x00000000);
    TLBWR();

    LDST((state_t *)0x0FFFF000);
	
}
void PassUpOrDie(int index){
	support_t * curr_support = current_proc->p_supportStruct;
	if(curr_support==NULL){
		SYSCALL_TERMINATEPROCESS(TERMINATE_CURR_PROCESS);
		return;
	}
	state_t * saved_exceptions_state = SAVED_EXCEPTIONS_STATE;
	state_copy(saved_exceptions_state, curr_support->sup_exceptState[index]);
	LDCXT(curr_support->sup_exceptContext[index].c_stackPtr,curr_support->sup_exceptContext[index].c_status,curr_support->sup_exceptContext[index].c_pc);
}
void Exception_handler(){
	uint_PTR exeCode = &current_proc->p_s.cause;
	*exeCode = CAUSE_GET_EXCCODE(*exeCode);
	if(*exeCode==EXC_INT){
		Interrupt_handler();
	}
	else if(TLB_EXCEPTION(*exeCode)){
		TLB_handler();
	}/*(*exeCode>=EXC_ADEL && *exeCode<=EXC_DBE) || (*exeCode>=EXC_BP && *exeCode<=EXC_OV)*/
	else if(TRAP_EXCEPTION(*exeCode))//Program Trap
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

	if((is_UM()) && IS_SYSCALL(current_proc->p_s.status)){/*significa che sei in user mode e non va bene, da chiedere al prof*/
		uint_PTR exeCode = &current_proc->p_s.cause;
		*exeCode = CAUSE_GET_EXCCODE(*exeCode); 
		*exeCode = EXC_RI;     /*setto il registro exeCode in RI e poi chiamo il program Trapp exception handler*/
		Prg_Trap_handler();
	}
	switch(current_proc->p_s.reg_a0){
		case CREATEPROCESS:
			SYSCALL_CREATEPROCESS((state_t *)*a1, (support_t *) *a2, (struct nsd_t *) *a3);
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
			SYSCALL_DOIO((int *)a1,(int *)a2);
			break;
		case GETTIME:
			SYSCALL_GETCPUTIME();
			break;
		case CLOCKWAIT:
			SYSCALL_WAITCLOCK();
			break;
		case GETSUPPORTPTR:
			SYSCALL_GET_SUPPORT_DATA();/*qua come faccio?*/
			break;
		case TERMINATE:
			SYSCALL_GETPID(*a1);
			break;
		case GET_TOD:
			SYSCALL_GETCHILDREN((int *)a1, *a2);
			break;
		default:  /*system call con a0 >= 11*/
			PassUpOrDie(GENERALEXCEPT);
			break;

	}
	
}
void Prg_Trap_handler(){
	PassUpOrDie(GENERALEXCEPT);
}
void TLB_handler(){
	PassUpOrDie(PGFAULTEXCEPT);
}

void BlockingSyscall(int *semaddr, pcb_t * process){
	
	state_t * saved_exceptions_state = SAVED_EXCEPTIONS_STATE;
	saved_exceptions_state->pc_epc += WORD_SIZE;
	state_copy(saved_exceptions_state, current_proc->p_s);
	/*DA FARE 
	Update the accumulated CPU time for the Current Process
	*/
	insertBlocked(semaddr, current_proc);
	/*scheduler();    chiamo lo scheduler   */

}

#pragma region SYSCALL_1-10
/*1*/
void SYSCALL_CREATEPROCESS(state_t *statep, support_t * supportp, struct nsd_t *ns){
	int pid;


	RETURN_SYSCALL(pid);
}
/*2*/
void SYSCALL_TERMINATEPROCESS (int pid){

}

/*3*/
void SYSCALL_PASSEREN (int *semaddr){
	if((*semaddr)>0){
		(*semaddr) --;
	}
	else{
		BlockingSyscall(semaddr, current_proc);
	}
}
/*4*/
void SYSCALL_VERHOGEN (int *semaddr){
	if((*semaddr)>=1){
		BlockingSyscall(semaddr, current_proc);
	}
	else{
		if(sem_queue_is_empty(semaddr)){
			(*semaddr)++;
		}
		else{
			pcb_t * awakened_process = removeBlocked(semaddr);
			struct list_head* head_rd = getHeadRd();
			insertProcQ(head_rd, awakened_process);
		}
		
		
	}
}

/*5*/
void SYSCALL_DOIO (int *cmdAddr, int *cmdValues){
	int ioStatus;

	RETURN_SYSCALL(ioStatus);
}

/*6*/
void SYSCALL_GETCPUTIME (){
	/*
	ancora da completare, bisogna gestire per bene il tempo
	*/
	RETURN_SYSCALL(current_proc->p_time);
	/*
	per tenere traccia del p_time si usa il timer TOD al quale si accede tramite questa macro
	STCK(x) dove x è una variabile cpu_t nella quale viene salvato il TOD 
	*/
/*
Hence, any return value described above (e.g. SYS6) needs to be put in
the specified register in the stored exception state
*/
	
}

/*7*/
void SYSCALL_WAITCLOCK(){
	/*
	insertBlocked(current_proc);
	*/
}

/*8*/
void SYSCALL_GET_SUPPORT_DATA(){


	RETURN_SYSCALL(current_proc->p_supportStruct);
}

/*9*/
void SYSCALL_GETPID( int parent){
	int pid;

	RETURN_SYSCALL(pid);
}
/*10*/
void SYSCALL_GETCHILDREN(int *children, int size){
	int pid; 


	RETURN_SYSCALL(pid);
}

#pragma endregion SYSCALL_1-10