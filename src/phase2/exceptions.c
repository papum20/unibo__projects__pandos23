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
/*semaforo dell'intervall timer, lo uso nella system call wait for clock 
  NOME TEMPORANEO   OSAMA lo deve modificare se vuole*/
extern semd_t * sem_IT;


void uTLB_RefillHandler() {

    setENTRYHI(0x80000000);
    setENTRYLO(0x00000000);
    TLBWR();

    LDST((state_t *)0x0FFFF000);
	
}



void Exception_handler(){
	uint_PTR exeCode = &current_proc->p_s.cause;
	*exeCode = CAUSE_GET_EXCCODE(*exeCode);

	if(*exeCode == EXC_INT){
		Interrupt_handler();
	}
	else if(TLB_EXCEPTION(*exeCode)){
	/* mi da undefined.. dov'è? (mi sa che l'hai tolto, o non so se è problema mio)*/
		TLB_handler();
	}
	else if(TRAP_EXCEPTION(*exeCode))//Program Trap
	/* uno spazio per il comment no? */
	{
		Prg_Trap_handler();
	}
	else if(*exeCode == EXC_SYS){
		SYSCALL_handler();
	}
	/* else? non so se sia definito una caso di default, o se ritornare qualcosa.. */

}


void SYSCALL_handler(){
	uint_PTR a1 = &current_proc->p_s.reg_a1;
	uint_PTR a2 = &current_proc->p_s.reg_a2;
	uint_PTR a3 = &current_proc->p_s.reg_a3;
	/* spunto:
	uint_PTR	a1 = &current_proc->p_s.reg_a1,
				a2 = &current_proc->p_s.reg_a2,
				a3 = &current_proc->p_s.reg_a3;
	*/

	if((is_UM()) && IS_SYSCALL(current_proc->p_s.status)){/*significa che sei in user mode e non va bene, da chiedere al prof*/
	/* parentesi non necessarie */
	/* if(is_UM() && IS_SYSCALL(current_proc->p_s.status)) { */
		uint_PTR exeCode = &current_proc->p_s.cause;
		/* non serve il puntatore al puntatore (&current_proc) */
		*exeCode = CAUSE_GET_EXCCODE(*exeCode); 
		/* assegnamento inutile, poi sovrascritto */
		/* 2 ambiguità:
			1. exeCode prima è unsigned int*, poi unsigned int
			2. exeCode prima è un registro cause (&current_proc->p_s.cause), poi un valore di exeCode
		*/
		/* e.g. solution:
			cause & !CAUSE_EXCCODE_MASK		// (and not) applica la (un)mask, cioè resetta la parte exc del registro cause
			// dopo ...
			cause | (EXC_RI << CAUSE_EXCCODE_BIT)	// setta la part exc del registro cause (dopo averlo azzerato)

			mettendo insieme:

			#define __CAUSE_EXCCODE_SETTED(cause, x) (cause & !CAUSE_EXCCODE_MASK) | (x << CAUSE_EXCCODE_BIT)
			static inline CAUSE_SET_EXCCODE(state_t *state, unsigned int code) {
				state.cause = __CAUSE_EXCCODE_SETTED(state.cause, code);
			}

			assegnamento: CAUSE_SET_EXCCODE(&current_proc->p_s);
		*/
		*exeCode = EXC_RI;     /*setto il registro exeCode in RI e poi chiamo il program Trap exception handler*/
		
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



void PassUpOrDie(int index){
	support_t * curr_support = current_proc->p_supportStruct;
	
	if(curr_support==NULL){
		SYSCALL_TERMINATEPROCESS(TERMINATE_CURR_PROCESS);
		return;
	}

	state_copy(SAVED_EXCEPTIONS_STATE, curr_support->sup_exceptState[index]);
	LDCXT(curr_support->sup_exceptContext[index].c_stackPtr, curr_support->sup_exceptContext[index].c_status, curr_support->sup_exceptContext[index].c_pc);
	/* return qui e non sopra */
}
/* il fatto che un'espressione così lunga sia ripetuta cosi tante volte dovrebbe far pensare: 
	curr_support->sup_exceptContext[index]

	per es.:
	1. curr_support: dargli nome più corto (purché sia ancora leggibile)
	2.
	void PassUpOrDie(int excpt_type) {
		if(current_proc->p_supportStruct == NULL)
			SYSCALL_TERMINATEPROCESS(TERMINATE_CURR_PROCESS);

		context_t *cxt = proc->p_supportStruct->sup_exceptContext[excpt_type];
		LDCXT(cxt.c_stackPtr, cxt.c_status, cxt.c_pc);
		return
	}
*/


void Prg_Trap_handler(){
	PassUpOrDie(GENERALEXCEPT);
}
/* già definite nell'h*/


void TLB_handler(){
	PassUpOrDie(PGFAULTEXCEPT);
}

/* funzioni così corte, e soprattutto senza parametri, hanno senso di essere macro,
	o quantomento, sicuramente, inline.
	*/



void BlockingSyscall(int *semaddr){
	RETURN_SYSCALL();
	/* un return come prima istruzione di una funzione?
		o è sbagliato,
		o lo scopo della macro è ben diverso da quello intuibile.
	*/
	//Update the accumulated CPU time for the Current Process
	update_p_time();
	insertBlocked(semaddr, current_proc);
	current_proc = NULL;
	Scheduler();

}


#pragma region SYSCALL_1_10

/*1*/
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
	
	/*questa system call ritorna il p_time + plus the amount of
CPU time used during the current quantum/time slice,   il clock PLT viene usato per dire quando il time slice del current
process finisce, quindi fra un time slice e un altro bisogna salvarsi il tempo*/

	__RETURN_SYSCALL(get_cpu_time() + get_CPU_time_slice_passed()); //questa funzione deve essere implementata da Tommaso e ritorna il tempo della CPU usato nel time slice
	RETURN_SYSCALL();
}


/*7*/
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

#pragma endregion SYSCALL_1_10