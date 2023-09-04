#include "interrupts.h" 




void Interrupt_handler(unsigned int cause) {

	//function that calculates the interrupt line
	int il = interrupt_line(cause);


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

void SYSCALL_DOIO_return(int *sem_key, unsigned int status) {

	//obtain the process from the relative semaphore
	pcb_t *pr = headBlocked(sem_key);

	//set its v0 register
	if (pr != NULL){
		pr->p_s.reg_v0 = status;
	}

	//remove it from the sem and add it to the readyQ
	SYSCALL_VERHOGEN(sem_key);

}

void Check_pending_interrupt(int line, int device){

	return *CDEV_BITMAP_ADDR(line) & (1 << device);

}

void Device_interrupt(int line){

	for(int i = 0; i < N_DEV_PER_IL; i++){
		if (Check_pending_interrupt(line, i)){

			//get the address of the device
			dtpreg_t *dev = (dtpreg_t*)DEV_REG_ADDR(line, i);

			//set the result on the process descriptor
			SYSCALL_DOIO_return(DEV_SEM_FROM_LINEDEV(line ,i), dev->status);

			//acknowledge the interrupt
			dev->command = ACK_DEVICE;
		}
	}

	//return control to the current process
	if (proc_curr != NULL)
		LDST( (state_t*)BIOSDATAPAGE);
	else
		Scheduler();

}

void IT_interrupt(){

	//Acknowledge the interrupt by loading the Interval Timer with 100 milliseconds
	LDIT(IT_RESET);

	//2. Unblock ALL pcbs blocked on the Pseudo-clock semaphore.
	pcb_t* unlockedProcess = NULL;
	while( (unlockedProcess = removeBlocked(&dev_sems[DEV_SEM_TIMER])) != NULL){
		
		insertProcQ(&readyQ, unlockedProcess);
		//aggiornare il tempo?

	}

	//Reset the Pseudo-clock semaphore to zero.
	dev_sems[DEV_SEM_TIMER] = 0;
	
	//Returnto the Current Process: Perform a LDST on the saved exception state
	if (proc_curr != NULL)
		LDST( (state_t*)BIOSDATAPAGE);
	else
		Scheduler();
}

void PLT_interrupt(){

	//ackownledge the PLT interrupt by loading the timer with a new value

	setTIMER(PLT_RESET);

	//copy the processor state at the time into current pcb
	proc_curr->p_s = *((state_t*)BIOSDATAPAGE);

	//place the process in readyqueue

	insertProcQ(&readyQ, proc_curr);

	//call the scheduler
	Scheduler();

}

void Terminal_interrupt(int line){

	for(int i = 0; i < N_DEV_PER_IL; i++){
		if (Check_pending_interrupt(line, i)){

			//get the terminal register
			termreg_t *reg = (termreg_t*)DEV_REG_ADDR(line, i);

			//give priority to the writing end of the terminal device
			if(reg->recv_status != TERM_READY){
				SYSCALL_DOIO_return(&dev_sems[SEM_INDEX_LD(TERM_WR_LINE, i)], reg->recv_status);
				reg->recv_command = ACK;
			}
			
			if(reg->transm_status != TERM_READY){
				SYSCALL_DOIO_return(&dev_sems[SEM_INDEX_LD(TERM_RD_LINE, i)], reg->transm_status);
				reg->transm_command = ACK;
			}
		}
	}

	if (proc_curr != NULL)
		LDST( (state_t*)BIOSDATAPAGE);
	else
		Scheduler();

}