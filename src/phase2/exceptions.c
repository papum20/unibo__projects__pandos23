/* notes:
1. Furthermore, SYSCALLs that do not result in process termination (even-
tually) return control to the process’s execution stream. This is done either
immediately (e.g. SYS6) or after the process is blocked and eventually un-
blocked (e.g. SYS5)
*/

#include "exceptions.h"
#include "exceptions_help.h"
#include "env_nucleus.h"






void Exception_handler(){
	memaddr exeCode = CAUSE_GET_EXCCODE(proc_curr->p_s.cause);
	if(exeCode == EXC_INT){
		Interrupt_handler();
	}
	else if(IS_TLB_EXCEPTION(exeCode)){
		TLB_handler();
	}
	else if(IS_TRAP_EXCEPTION(exeCode))
	{
		Prg_Trap_handler();
	}
	else if(exeCode == EXC_SYS){
		SYSCALL_handler();
	}
	/* else? non so se sia definito una caso di default, o se ritornare qualcosa.. sul libro non dice di un valore di default */

}


void SYSCALL_handler(){
	unsigned int	a0 = A0(proc_curr),
					a1 = A1(proc_curr),
					a2 = A2(proc_curr),
					a3 = A3(proc_curr);
	
	if(is_UM()) {	/*significa che sei in user mode e non va bene*/
		CAUSE_SET_EXCCODE(proc_curr->p_s, EXC_RI); /*setto il registro exeCode in RI e poi chiamo il program Trap exception handler*/		
		Prg_Trap_handler();
		return;
	}
	
	if(a0 >= CREATEPROCESS && a0 <= GET_TOD) {
		SYSCALL(a0, a1, a2, a3);
	} else {	/*system call con a0 >= 11*/
		PassUpOrDie(GENERALEXCEPT);
	}

}

	/*
	prova questo:

	#include <stdio.h>
	#define DOIO 1
	#define VERHOGEN 2
	#define SYSCALL(number, a0, a1) SYSCALL_##number(a0, a1)

	void SYSCALL_DOIO(int a0, int a1) {
		printf("DOIO %d %d\n", a0, a1);
	}
	void SYSCALL_VERHOGEN(int a0, int a1) {
		printf("V %d %d\n", a0, a1);

	}

	int main() {
		SYSCALL(DOIO, 1, 2);
		SYSCALL(VERHOGEN, 3, 2);
		int a = 5, b = 6;
		SYSCALL(VERHOGEN, a, b);
	}
	*/
	


void PassUpOrDie(int excpt_type) {
		support_t * curr_sup = proc_curr->p_supportStruct;
		if(curr_sup == NULL){
			SYSCALL(TERMPROCESS, TERMINATE_CURR_PROCESS, NULL, NULL);
			return;
		}
			
		STATE_CP(*SAVED_EXCEPTIONS_STATE, &(curr_sup->sup_exceptState[excpt_type]) );
		context_t cxt = curr_sup->sup_exceptContext[excpt_type];
		LDCXT(cxt.c_stackPtr, cxt.c_status, cxt.c_pc);
}



void uTLB_RefillHandler() {

    setENTRYHI(0x80000000);
    setENTRYLO(0x00000000);
    TLBWR();

    LDST((state_t *)0x0FFFF000);
	
}



/*
 * SYSTEM CALLS
 */

/*
* 1
*/
void SYSCALL_CREATEPROCESS(state_t *statep, support_t * supportp, struct nsd_t *ns) {
	pcb_t *new_proc = allocPcb();

	/* if there are no free pcbs, return -1 */
	if(new_proc == NULL) {
		RETURN_SYSCALL(ERR);
	}

	/* init pcb fields */
	__init_createProc(new_proc, proc_curr, statep, supportp, ns);
	/* other fields already were set to 0/NULL by allocPcb() */
	
	/* update Nucleus variables */
	insertProcQ(&readyQ, new_proc);
	proc_alive_n++;

	RETURN_SYSCALL(PID(new_proc));
}

/*
* 2
*/
void SYSCALL_TERMINATEPROCESS (int pid) {
	
	if(pid == 0)
		__terminateTree(proc_curr);
	else
		__terminateTree(pcb_from_PID(pid));

	if(proc_curr == NULL)
		/* current process was terminated */
		Scheduler();
}

/* 3
*/
void SYSCALL_PASSEREN (int *semaddr) {

	(*semaddr)--;
	
	if(*semaddr > 0) {
		/* no process is blocked on a P */
		
		if(*semaddr > 1)
			/* wake up a process blocked on a V */
			removeBlocked(semaddr);

		RETURN_SYSCALL_VOID();
	}
	else
		RETURN_SYSCALL_BLOCK(semaddr);

}

/* 4
*/
void SYSCALL_VERHOGEN (int *semaddr) {

	(*semaddr)++;
	
	/* blocking V, as it's a binary semaphore */
	if(*semaddr >= 1) {
		RETURN_SYSCALL_BLOCK(semaddr);
		return;
	}

	if(*semaddr < 0) {
		/* wake up a process blocked on a P */
		pcb_t *awakened_proc = removeBlocked(semaddr);
		insertProcQ(&readyQ, awakened_proc);
	}

	RETURN_SYSCALL_VOID();
}

/* 5
*/
void SYSCALL_DOIO (int *cmdAddr, int *cmdValues){

	int ioStatus;
	RETURN_SYSCALL(ioStatus);
}


/* 6
*/
void SYSCALL_GETCPUTIME() {

	RETURN_SYSCALL(get_cpu_time() + get_CPU_time_slice_passed());
}

/* 7
*/
void SYSCALL_WAITCLOCK() {

	/* block the process on the interval timer semaphore */
	RETURN_SYSCALL_BLOCK(DEV_SEM(SEM_TIMER));
}

/* 8
*/
void SYSCALL_GET_SUPPORT_DATA(){

	RETURN_SYSCALL((memaddr)(&proc_curr->p_supportStruct));
}

/* 9
*/
void SYSCALL_GETPID(int parent) {

	int pid;
	
	if(parent != TRUE)
		pid = PID(proc_curr);
	else if(getNamespace(proc_curr, NS_PID) == getNamespace(proc_curr->p_parent, NS_PID))
		pid = PID(proc_curr->p_parent);
	else
		pid = 0;

	RETURN_SYSCALL(pid);
}


/* 10
*/
void SYSCALL_GETCHILDREN(int *children, int size){
	int number_of_children = 0; 
	int index = 0;
	/*scorro tutta la lista dei figli del current_proc e metto nell'array children tutti i pid dei figli che sono nello stesso namespace*/
	
	if(!emptyChild(proc_curr)){
		struct list_head *pos;
		
		list_for_each(pos, &proc_curr->p_child) { 
			pcb_t *child = container_of(pos, pcb_t, p_sib); 
			if(getNamespace(child, NS_PID)==getNamespace(proc_curr,NS_PID)){
				if(index<size){
					children[index] = GET_PROC_PID(child);
					index++;
				}
				/*
				else{
					RETURN_SYSCALL(number_of_children);
					return;
				}*/
			}
		}
	}
	RETURN_SYSCALL(number_of_children);
}




