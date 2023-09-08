#include "interrupts.h" 




void Interrupt_handler() {

	//function that calculates the interrupt line
	int il = interrupt_line(SAVED_EXCEPTION_STATE->cause);

	if (il == 1){
		PLT_interrupt();							//PLT interrupt handler
	}
	else if (il == 2){
		IT_interrupt();								//IT interrupt handler
	}
	else if (il == 7){
		Terminal_interrupt(il);				//Terminals have a peculiar handling strategy
	}
	else if (il > 2 &&  il < 7){
		Device_interrupt(il);					//Interrupt lines from 3 to 6 have the same handling strategy
	}
	else {
		PANIC();											//If the interrupt line is not between 1 and 7, let's Panic
	}
}

/* taking advantage of `status` field being always the first one for all (sub)devices */
void SYSCALL_DOIO_return(int *sem_key, unsigned int *reg) {

	//obtain the process from the relative semaphore
	pcb_t *pr = headBlocked(sem_key);

	if (pr != NULL){
		proc_soft_blocked_n--;
		/* return status code (in cmdValues, whose address is still in its a1) */
		*(memaddr *)(pr->p_s.reg_a1) = *reg;
		/* return code */
		pr->p_s.reg_v0 = (DEV_STATUS(*reg) == TERM_SUCCESS) ?
			RETURN_DOIO_SUCCESS : RETURN_DOIO_FAILURE;
		/* return values (copy device register in cmdValues, still in a2) */
		for(int i = 0; i < ((DEV_IL((int)reg) == IL_TERMINAL) ? N_ARGS_TERM : N_ARGS_DEV); i++)
			*((memaddr *)pr->p_s.reg_a2 + i) = *(reg + i);
	}

	//remove it from the sem and add it to the readyQ
	SYSCALL_VERHOGEN(sem_key, TRUE);

}



int Check_pending_interrupt(int line, int device){

	return (*(memaddr*)CDEV_BITMAP_ADDR(line)) & (1 << device);

}



void PLT_interrupt(){

	//ackownledge the PLT interrupt by loading the timer with a new value

	setTIMER(PLT_RESET);

	//copy the processor state at the time into current pcb
	STST(&proc_curr->p_s);

	//place the process in readyqueue

	insertProcQ(&readyQ, proc_curr);

	//call the scheduler
	Scheduler();

}

void IT_interrupt(){

	//Acknowledge the interrupt by loading the Interval Timer with 100 milliseconds
	LDIT(IT_RESET);

	//2. Unblock ALL pcbs blocked on the Pseudo-clock semaphore.
	pcb_t* unlockedProcess = NULL;
	while( (unlockedProcess = removeBlocked(DEV_SEM_TIMER)) != NULL){
		
		insertProcQ(&readyQ, unlockedProcess);
		//aggiornare il tempo?

	}

	//Reset the Pseudo-clock semaphore to zero.
	*DEV_SEM_TIMER = 0;
	
	//Return to the Current Process: Perform a LDST on the saved exception state
	if (proc_curr != NULL)
		LDST( (state_t*)BIOSDATAPAGE);
	else
		Scheduler();
}


void Terminal_interrupt(int line){

	for(int i = 0; i < N_DEV_PER_IL; i++){
		if (Check_pending_interrupt(line, i)){

			//get the terminal register
			termreg_t *reg = (termreg_t*)DEV_REG_ADDR(line, i);

			//give priority to the writing end of the terminal device
			if(DEV_STATUS(reg->recv_status) != DEV_READY){
				SYSCALL_DOIO_return(EXT_DEV_SEM_FROM_REGADDR((memaddr)(&reg->recv_command)), &reg->recv_status);
				DEV_CMD_SET(reg->recv_command, ACK);
			}
			
			if(DEV_STATUS(reg->transm_status) != DEV_READY){
				SYSCALL_DOIO_return(EXT_DEV_SEM_FROM_REGADDR((memaddr)(&reg->transm_command)), &reg->transm_status);
				DEV_CMD_SET(reg->transm_command, ACK);
			}
		}
	}

	if (proc_curr != NULL)
		LDST( (state_t*)BIOSDATAPAGE);
	else
		Scheduler();

}

void Device_interrupt(int line){

	for(int i = 0; i < N_DEV_PER_IL; i++){
		if (Check_pending_interrupt(line, i)){

			//get the address of the device register
			dtpreg_t *reg = (dtpreg_t*)DEV_REG_ADDR(line, i);

			//set the result on the process descriptor
			SYSCALL_DOIO_return(EXT_DEV_SEM_FROM_REGADDR((devregtr)reg), reg);

			//acknowledge the interrupt
			reg->command = ACK_DEVICE;
		}
	}

	//return control to the current process
	if (proc_curr != NULL)
		LDST( (state_t*)BIOSDATAPAGE);
	else
		Scheduler();

}