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
/* ok, poi metticene di giusti/"non rozzi" (cioè che dicono cosa sono le variabili) */

	/*questo lo metto extern come mi ha suggerito Daniele così si collega al suo puntatore del current_proc del suo file init, per 
	questo qui non devo mettergli valore ma solo dichiararlo o accederci */
	extern pcb_t * proc_curr;
	/*mi serve per la prima system call*/
	extern int proc_alive_n;
	extern int proc_soft_blocked_n;
	/*semaforo dell'intervall timer, lo uso nella system call wait for clock 
	NOME TEMPORANEO   OSAMA lo deve modificare se vuole*/
	extern semd_t * sem_IT;
	
	extern struct list_head readyQ;




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
	unsigned int	a0 = proc_curr->p_s.reg_a0,
					a1 = proc_curr->p_s.reg_a1,
					a2 = proc_curr->p_s.reg_a2,
					a3 = proc_curr->p_s.reg_a3;
	
	if(is_UM() && IS_SYSCALL(proc_curr->p_s.status)) {	/*significa che sei in user mode e non va bene*/
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
			
		state_copy(SAVED_EXCEPTION_STATE, curr_sup->sup_exceptState[excpt_type]);
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
	update_p_time();
	insertBlocked(semaddr, proc_curr);
	proc_curr = NULL;
	Scheduler();

}


/*
 * SYSTEM CALL
 */

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
	state_copy(statep, new_proc->p_s);/*stetep è lo stato del nuovo processo e quindi lo copio in p_s del new_proc*/

	/*il nuovo pcb è messo nella ready queue e figlio del pcb corrente*/
	insertChild(proc_curr, new_proc);
	insertProcQ(&readyQ, new_proc);

	if(ns!=NULL){//ns diventa il namespace di new_proc
		addNamespace(new_proc, ns);
	}
	else{//eredita il namespace del padre
		addNamespace(new_proc, getNamespace(proc_curr,NS_PID));
	}

	proc_alive_n++;
	pid = GET_PROC_PID(new_proc);  /*il pid è l'indirizzo di memoria dove è salvato il pcb*/
	/*## dove l'hai visto che il pid si ottiene cosi? il libro ha suggerito di farlo in questo modo
	This unique identifier(Il pid associato a un pcb) can be the address of the PCB structure or a sequential value. 
	The only constraints on this value are that this value must be not null (0) and there shall not be two processes with the same identifier.*/
	UPDATE_REGV0(pid);
	RETURN_SYSCALL();
}


/*
* 2
*/
void SYSCALL_TERMINATEPROCESS (int pid){
	pcb_t * proc_to_terminate;
	if(pid==0 || pid==NULL){
		proc_to_terminate = proc_curr;
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
	outChild(proc_to_terminate); 

	if(proc_to_terminate==proc_curr){ /*il processo corrente non può essere bloccato in un semaforo quindi non considero sta cosa*/
		freePcb(proc_to_terminate); 
		proc_curr = NULL;
		Scheduler();     /*chiamo lo scheduler per decidere quale processo mettere in current_proc*/
	}
	else{
		if(proc_to_terminate->p_semAdd!=NULL){ /*controllo se il processo sia bloccato ad un semaforo non device*/
			//Rimuovo il processo dal semaforo
			outBlocked(proc_curr);
			//Rilascio il semaforo
			*(proc_curr->p_semAdd) +=1;
		}
		else if(is_blocked_by_devSem(proc_to_terminate)){ //questo else if dipende molto dall'implementazione di Osama, da rivedere
			//Rimuovo il processo dal semaforo
			outBlocked(proc_curr);
			proc_soft_blocked_n--;
		}
		else{/*vedo se è nella coda ready*/
			//Rimuovo il processo dalla coda ready
			outProcQ(&readyQ, proc_to_terminate);
		}
	}
	freePcb(proc_to_terminate); 
	proc_alive_n--; 
}


/*
* 3
*/
void SYSCALL_PASSEREN (int *semaddr){
	semd_t * sem = (semd_t *)semaddr;
	if( *sem->s_key > 0){
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
	if((*sem->s_key)>=1){//essendo un semaforo binario pure la V blocca
		RETURN_SYSCALL();
		BlockingSyscall(sem->s_key);
	}
	else{
		if(emptyProcQ(&sem->s_procq) == true){
			(*sem->s_key)++;

		}
		else{//risveglio un processo dalla coda dei bloccati
			pcb_t * awakened_process = removeBlocked(sem->s_key);
			insertProcQ(&readyQ, awakened_process);
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
	UPDATE_REGV0(proc_curr->p_supportStruct);
	RETURN_SYSCALL();
}


/*
* 9
*/
void SYSCALL_GETPID( int parent){
	int pid;
	if(parent==TRUE){//vogliamo il pid del padre del current_process
		/*
		se il current_process non è nello stesso namespace del padre allora questa system call ritorna 0
		*/
		if(getNamespace(proc_curr, NS_PID)!=getNamespace(proc_curr->p_parent, NS_PID)){
			pid = 0;
		}
		else{
			pid = GET_PROC_PID(proc_curr->p_parent);
		}
	}
	else{//vogliamo il pid del current_process
		pid = GET_PROC_PID(proc_curr);
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
	/*scorro tutta la lista dei figli del current_proc e metto nell'array children tutti i pid dei figli che sono nello stesso namespace*/
	if(!emptyChild(proc_curr)){
		struct list_head *pos;
	list_for_each(pos, &proc_curr->p_child){ 
		pcb_t *child = container_of(pos, pcb_t, p_sib); 
		if(getNamespace(child, NS_PID)==getNamespace(proc_curr,NS_PID)){
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



