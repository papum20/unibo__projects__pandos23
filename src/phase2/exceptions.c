/*
* exceptions.c
* This module implements the TLB, Program Trap, and
* SYSCALL exception handlers. Furthermore, this module will contain
* the provided skeleton TLB-Refill event handler (e.g. uTLB_RefillHandler).
*/


#include "exceptions.h"

/*
* COMMENTI TEMPORANEI MOLTO ROZZI CHE HO SCRITTO ORA SOLO PER RICORDARMI COSA SONO, DA RIFINIRE IN SEGUITO
*/
	/*questo lo metto extern come mi ha suggerito Daniele così si collega al suo puntatore del current_proc del suo file init, per 
	questo qui non devo mettergli valore ma solo dichiararlo o accederci */
	extern pcb_t * current_proc;
	/*mi serve per la prima system call*/
	extern int proc_count;
	extern int soft_block_count;
	/*semaforo dell'intervall timer, lo uso nella system call wait for clock 
	NOME TEMPORANEO   OSAMA lo deve modificare se vuole*/
	extern semd_t * sem_IT;




void Exception_handler(){
	memaddr exeCode = CAUSE_GET_EXCCODE(current_proc->p_s.cause);
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
	/* else? non so se sia definito una caso di default, o se ritornare qualcosa.. */

}


void SYSCALL_handler(){
	memaddr	a1 = current_proc->p_s.reg_a1,
			a2 = current_proc->p_s.reg_a2,
			a3 = current_proc->p_s.reg_a3;
	
	if(is_UM() && IS_SYSCALL(current_proc->p_s.status)) {/*significa che sei in user mode e non va bene*/
		CAUSE_SET_EXCCODE(current_proc->p_s, EXC_RI); /*setto il registro exeCode in RI e poi chiamo il program Trap exception handler*/		
		Prg_Trap_handler();
		return;
	}
	
	switch(current_proc->p_s.reg_a0){
		case CREATEPROCESS:
			SYSCALL_CREATEPROCESS((state_t *)a1, (support_t *) a2, (struct nsd_t *) a3);
			break;
		case TERMPROCESS:
			SYSCALL_TERMINATEPROCESS (a1);
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
			SYSCALL_GET_SUPPORT_DATA();
			break;
		case TERMINATE:
			SYSCALL_GETPID(a1);
			break;
		case GET_TOD:
			SYSCALL_GETCHILDREN((int *)a1, a2);
			break;
		default:  /*system call con a0 >= 11*/
			PassUpOrDie(GENERALEXCEPT);
			break;

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
	
}


void PassUpOrDie(int excpt_type) {
		support_t * curr_sup = current_proc->p_supportStruct;
		if(curr_sup == NULL){
			SYSCALL_TERMINATEPROCESS(TERMINATE_CURR_PROCESS);
			return;
		}
			
		state_copy(SAVED_EXCEPTIONS_STATE, curr_sup->sup_exceptState[excpt_type]);
		context_t cxt = curr_sup->sup_exceptContext[excpt_type];
		LDCXT(cxt.c_stackPtr, cxt.c_status, cxt.c_pc);
}



void uTLB_RefillHandler() {

    setENTRYHI(0x80000000);
    setENTRYLO(0x00000000);
    TLBWR();

    LDST((state_t *)0x0FFFF000);
	
}


void BlockingSyscall(int *semaddr){
	//Update the accumulated CPU time for the Current Process
	update_p_time();
	insertBlocked(semaddr, current_proc);
	current_proc = NULL;
	Scheduler();

}


#pragma region SYSCALL_1_10


/*
* 1
*/
void SYSCALL_CREATEPROCESS(state_t *statep, support_t * supportp, struct nsd_t *ns){
	int pid;
	pcb_t * new_proc = allocPcb();
	if(new_proc == NULL){
		UPDATE_REGV0(-1); //se non ci sono free pcb ritorni -1
		RETURN_SYSCALL();

	} 
	state_copy(statep, new_proc->p_s);/*stetep è lo stato del nuovo processo*/

	/*il nuovo pcb è messo nella ready queue e figlio del pcb corrente*/
	struct list_head* head_rd = getHeadRd();
	insertChild(current_proc, new_proc);
	insertProcQ(head_rd, new_proc);
	/*p_time is set to zero;*/
	new_proc->p_time=0;
	/*p_supportStruct from a2*/ /*If no parameter is provided, this field is
									set to NULL Non ho capito sta parte*/
	new_proc->p_supportStruct=supportp;
	new_proc->p_semAdd=NULL;
	if(ns!=NULL){
		new_proc->namespaces[NS_PID]=ns;
	}
	else{
		new_proc->namespaces[NS_PID]=getNamespace(current_proc,NS_PID);
	}
	/*process count è incrementato di 1*/
	proc_count++;
	pid = GET_PROC_PID(new_proc);  /*il pid è l'indirizzo di memoria dove è salvato il pid*/
	UPDATE_REGV0(pid);
	RETURN_SYSCALL();
}


/*
* 2
*/
void SYSCALL_TERMINATEPROCESS (int pid){
	pcb_t * proc_to_terminate;
	if(pid==0 || pid==NULL){
		proc_to_terminate = current_proc;
	}
	else{
		proc_to_terminate = GET_PCB_FROM_PID(pid);
	}
	/*uccido i figli*/
	if(!emptyChild(proc_to_terminate)){
		pcb_t * child = returnChild(proc_to_terminate);
		/*itero ricorsivamente sui figli*/
		while(child!=NULL){
			SYSCALL_TERMINATEPROCESS(GET_PROC_PID(child));
			child = returnChild(proc_to_terminate);
		}
	}
	/*se proc non ha figli lo stacco dal padre*/
	outChild(proc_to_terminate); //proc non ha più il padre

	if(proc_to_terminate==current_proc){ /*il processo corrente non può essere bloccato in un semaforo quindi non considero sta cosa*/
		freePcb(proc_to_terminate); /*libero il pcb*/
		current_proc = NULL;
		Scheduler();     /*chiamo lo scheduler per decidere quale processo mettere in current_proc*/
	}
	else{
		if(proc_to_terminate->p_semAdd!=NULL){ /*controllo se il processo sia bloccato ad un semaforo non device*/
			//Rimuovo il processo dal semaforo
			outBlocked(current_proc);
			//Rilascio il semaforo
			*(current_proc->p_semAdd) +=1;
		}
		else if(is_blocked_by_devSem(proc_to_terminate)){ //questo else if dipende molto dall'implementazione di Osama, da rivedere
			//Rimuovo il processo dal semaforo
			outBlocked(current_proc);
			soft_block_count--;
		}
		else{/*vedo se è nella coda ready*/
			struct list_head* head_rd = getHeadRd();
			//Rimuovo il processo dalla coda ready
			outProcQ(head_rd, proc_to_terminate);
		}
	}
	freePcb(proc_to_terminate); /*libero il pcb*/
	proc_count--; /*decremento proc_count*/
}


/*
* 3
*/
void SYSCALL_PASSEREN (int *semaddr){
	semd_t * sem = (semd_t *)semaddr;
	if((*sem->s_key)>0){
		(*sem->s_key) --;
		RETURN_SYSCALL();
	}
	else{
		RETURN_SYSCALL();
		BlockingSyscall(sem->s_key);
	}
}


/*
* 4
*/
void SYSCALL_VERHOGEN (int *semaddr){
	semd_t * sem = (semd_t *)semaddr;
	if((*sem->s_key)>=1){
		RETURN_SYSCALL();
		BlockingSyscall(sem->s_key);
	}
	else{
		if(emptyProcQ(&sem->s_procq) == true){
			(*sem->s_key)++;

		}
		else{
			pcb_t * awakened_process = removeBlocked(sem->s_key);
			struct list_head* head_rd = getHeadRd();
			insertProcQ(head_rd, awakened_process);
		}
		RETURN_SYSCALL();
	}
}


/*
* 5
*/
void SYSCALL_DOIO (int *cmdAddr, int *cmdValues){
	int ioStatus;

	UPDATE_REGV0(ioStatus);
	RETURN_SYSCALL();
}


/*
* 6
*/
void SYSCALL_GETCPUTIME (){
	
	/*questa system call ritorna il p_time + plus the amount of
CPU time used during the current quantum/time slice,   il clock PLT viene usato per dire quando il time slice del current
process finisce, quindi fra un time slice e un altro bisogna salvarsi il tempo*/

	UPDATE_REGV0(get_cpu_time() + get_CPU_time_slice_passed()); 
	RETURN_SYSCALL();
}


/*
* 7
*/
void SYSCALL_WAITCLOCK(){
	RETURN_SYSCALL();
	BlockingSyscall(sem_IT->s_key);
}


/*
* 8
*/
void SYSCALL_GET_SUPPORT_DATA(){
	UPDATE_REGV0(current_proc->p_supportStruct);
	RETURN_SYSCALL();
}


/*
* 9
*/
void SYSCALL_GETPID( int parent){
	int pid;
	if(parent==TRUE){//we want the PID of the parent of the calling process
		/*
		if a
		process is not in the same PID namespace of its parent, this systemcall, with
		the correct request of the parent PID, will return 0
		*/
		if(getNamespace(current_proc, NS_PID)!=getNamespace(current_proc->p_parent, NS_PID)){
			pid = 0;
		}
		else{
			pid = GET_PROC_PID(current_proc->p_parent);
		}
	}
	else{//we want the PID of the calling process
		pid = GET_PROC_PID(current_proc);
	}
	UPDATE_REGV0(pid);
	RETURN_SYSCALL();
}


/*
* 10
*/
void SYSCALL_GETCHILDREN(int *children, int size){
	int number_of_children = 0; 
	int index = 0;
	/*scorro tutta la lista dei figli del current_proc*/
	if(!emptyChild(current_proc)){
		struct list_head *pos;
	list_for_each(pos, &current_proc->p_child){ 
		pcb_t *child = container_of(pos, pcb_t, p_sib); 
		if(getNamespace(child, NS_PID)==getNamespace(current_proc,NS_PID)){
			if(index<size){
				children[index] = GET_PROC_PID(child);
				index++;
			}
			else{
				UPDATE_REGV0(number_of_children);
				RETURN_SYSCALL();
				return;
			}
		}
	}
	}
	UPDATE_REGV0(number_of_children);
	RETURN_SYSCALL();
}

#pragma endregion SYSCALL_1_10


