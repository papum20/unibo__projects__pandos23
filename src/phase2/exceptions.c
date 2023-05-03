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
/*mi serve per la prima system call*/
extern int proc_count;
extern int soft_block_count;
/*semaforo dell'intervall timer, lo uso nella system call wait for clock */
extern semd_t * sem_IT;



/********
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/

/*
	dice se la coda dei processi bloccati relativa al semaforo è vuota
*/
inline int sem_queue_is_empty(int *semAdd){														
	semd_t *sem = __hash_semaphore(semAdd);
	return emptyProcQ(&sem->s_procq) == true;	

}

/********
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
*/





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
	LDCXT(curr_support->sup_exceptContext[index].c_stackPtr, curr_support->sup_exceptContext[index].c_status, curr_support->sup_exceptContext[index].c_pc);
}
void Exception_handler(){
	uint_PTR exeCode = &current_proc->p_s.cause;
	*exeCode = CAUSE_GET_EXCCODE(*exeCode);
	if(*exeCode == EXC_INT){
		Interrupt_handler();
	}
	else if(TLB_EXCEPTION(*exeCode)){
		TLB_handler();
	}
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
			SYSCALL_GET_SUPPORT_DATA();
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

/*
	macro ausiliaria che gestisce dove mettere il valore di ritorno delle system call che ritornano qualcosa
	essa mette nel registro v0 del current process il pid di un processo passato come parametro
*/
#define __RETURN_SYSCALL(x) (current_proc->p_s.reg_v0 = (memaddr)x)

/*
	funzione che gestisce il ritorno di una system call
	aggiorna il saved exception state aumentando program counter di una word per evitare loop e aggiornando il CPU Time del processo corrente
*/

inline void RETURN_SYSCALL(){
	state_t * saved_exceptions_state = SAVED_EXCEPTIONS_STATE;
	saved_exceptions_state->pc_epc += WORDLEN;
	state_copy(saved_exceptions_state, current_proc->p_s);
}


void BlockingSyscall(int *semaddr){
	RETURN_SYSCALL();
	/*DA FARE 
	Update the accumulated CPU time for the Current Process
	*/
	insertBlocked(semaddr, current_proc);
	current_proc = NULL;
	Scheduler();

}

#pragma region SYSCALL_1-10
/*1 se tutto va bene ritorna il pid del processo creato*/
void SYSCALL_CREATEPROCESS(state_t *statep, support_t * supportp, struct nsd_t *ns){
	int pid;
	pcb_t * new_proc = allocPcb();
	if(new_proc == NULL){
		__RETURN_SYSCALL(-1); //se non ci sono free pcb ritorni -1
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
	__RETURN_SYSCALL(pid);
	RETURN_SYSCALL();
}
/*2*/
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

/*3*/
void SYSCALL_PASSEREN (int *semaddr){
	semd_t * sem = (semd_t *)semaddr;
	if((*sem->s_key)>0){
		(*sem->s_key) --;
		RETURN_SYSCALL();
	}
	else{
		BlockingSyscall(sem->s_key);
	}
}
/*4*/
void SYSCALL_VERHOGEN (int *semaddr){
	semd_t * sem = (semd_t *)semaddr;
	if((*sem->s_key)>=1){
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

/*5*/
void SYSCALL_DOIO (int *cmdAddr, int *cmdValues){
	int ioStatus;

	__RETURN_SYSCALL(ioStatus);
	RETURN_SYSCALL();
}

/*6*/
void SYSCALL_GETCPUTIME (){
	
	/*
	ancora da completare, bisogna gestire per bene il tempo
	*/
	__RETURN_SYSCALL(current_proc->p_time);
	RETURN_SYSCALL();
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
/*bisogna usare lo speudo clock cioè l'interval timer che io rendo da zero a numeri negativi e poi l'interrupt handler riporta a zero ogni tot*/
void SYSCALL_WAITCLOCK(){
	BlockingSyscall(sem_IT->s_key);
}

/*8*/
void SYSCALL_GET_SUPPORT_DATA(){
	__RETURN_SYSCALL(current_proc->p_supportStruct);
	RETURN_SYSCALL();
}

/*9*/
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
	__RETURN_SYSCALL(pid);
	RETURN_SYSCALL();
}
/*10*/
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
				__RETURN_SYSCALL(number_of_children);
				RETURN_SYSCALL();
				return;
			}
		}
	}
	}
	__RETURN_SYSCALL(number_of_children);
	RETURN_SYSCALL();
}

#pragma endregion SYSCALL_1-10