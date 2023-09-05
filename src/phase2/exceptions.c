/* notes:
1. Furthermore, SYSCALLs that do not result in process termination (even-
tually) return control to the process’s execution stream. This is done either
immediately (e.g. SYS6) or after the process is blocked and eventually un-
blocked (e.g. SYS5)
*/

#include "exceptions.h"

#include "env_nucleus.h"

#include "exceptions_help.h"
#include "pcb_help.h"



void Exception_handler(){

	unsigned int exeCode = CAUSE_GET_EXCCODE(SAVED_EXCEPTION_STATE->cause);
	
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
	/* no default fallback case */
}


void SYSCALL_handler(){
	unsigned int	a0 = A0(proc_curr),
					a1 = A1(proc_curr),
					a2 = A2(proc_curr),
					a3 = A3(proc_curr);
	
	if(IS_UM) {
		CAUSE_SET_EXCCODE(EXC_RI);	
		Prg_Trap_handler();
		return;
	}
	
	if(a0 >= CREATEPROCESS && a0 <= GET_TOD) {
		_SYSCALL(a0, a1, a2, a3);
	} else {
		/* system call con a0 >= 11 */
		PassUpOrDie(GENERALEXCEPT);
	}

}
	


void PassUpOrDie(int excpt_type) {

		support_t * curr_sup = proc_curr->p_supportStruct;
		if(curr_sup == NULL){
			SYSCALL_TERMINATEPROCESS(SYSCALL_TERMINATE_CURR_PROC);
			return;
		}
			
		STATE_CP( *SAVED_EXCEPTION_STATE, &(curr_sup->sup_exceptState[excpt_type]) );
		context_t cxt = curr_sup->sup_exceptContext[excpt_type];
		LDCXT(cxt.stackPtr, cxt.status, cxt.pc);
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

/* 1
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

/* 2
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
	
	if(*semaddr >= 0) {
		/* no process is blocked on a P */
		
		if(*semaddr >= 1)
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
	if(*semaddr > 1) {
		RETURN_SYSCALL_BLOCK(semaddr);
		return;
	}

	if(*semaddr <= 0) {
		/* wake up a process blocked on a P */
		pcb_t *awakened_proc = removeBlocked(semaddr);
		insertProcQ(&readyQ, awakened_proc);
	}

	RETURN_SYSCALL_VOID();
}

/* 5
*/
void SYSCALL_DOIO (int *cmdAddr, int *cmdValues){

	/* pass the parameters to the device */
	dev_setArgs(cmdAddr, cmdValues);
	/* get the corresponding semaphore */
	int *sem = EXT_DEV_SEM_FROM_REGADDR((devregtr)cmdAddr);
	/* block the process */
	SYSCALL_PASSEREN(sem);
}

/* 6
*/
void SYSCALL_GETCPUTIME() {

	RETURN_SYSCALL(CPU_TIME_USED(proc_curr));
}

/* 7
*/
void SYSCALL_WAITCLOCK() {

	/* block the process on the interval timer semaphore */
	RETURN_SYSCALL_BLOCK(DEV_SEM_TIMER);
}

/* 8
*/
void SYSCALL_GET_SUPPORT_DATA() {

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

	int n = 0;
	struct pcb_t *child;
		
	pcb_for_each_child(child, proc_curr) { 

		if(getNamespace(child, NS_PID) == getNamespace(proc_curr, NS_PID)) {
			if(n < size)
				children[n++] = PID(child);
			n++;
		}
	}

	RETURN_SYSCALL(n);
}




