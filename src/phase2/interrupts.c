#include "interrupts.h" 




void Interrupt_handler(unsigned int cause) {
	/*
	Non-Timer Interrupts:
	1. Calculate the address for this device’s device register. [Section ??-pops]

	2. Save off the status code from the device’s device register.

	3. Acknowledge the outstanding interrupt. This is accomplished by writ-
	ing the acknowledge command code in the interrupting device’s device
	register. Alternatively, writing a new command in the interrupting
	device’s device register will also acknowledge the interrupt.
	*/
	int il = interrupt_line(cause);

	if (il == 1){
		PLT_interrupt();
	}
	else if (il == 2){
		IT_interrupt();
	}
	else if (il == 7){
		Terminal_interrupt();
	}
	else if (il > 2 &&  il < 7){
		Device_interrupt(il);
	}
	else {
		PANIC();
	}
}

void SYSCALL_DOIO_return() {
	/*
	Non-Timer Interrupts:
	4. Perform a V operation on the Nucleus maintained semaphore associ-
	ated with this (sub)device. This operation should unblock the process
	(pcb) which initiated this I/O operation and then requested to wait for
	its completion via a SYS5 operation.

	5. Place the stored off status code in the newly unblocked pcb’s v0 register.
	*/

	/*
	6. Insert the newly unblocked pcb on the Ready Queue, transitioning this
	process from the “blocked” state to the “ready” state.

	7. Return control to the Current Process: Perform a LDST on the saved
	exception state (located at the start of the BIOS Data Page [Section
	*/
}

extern void Check_pending_interrupt(int line, int device){
	return *INTR_CURRENT_BITMAP(line) & (1 << device);
}

void Device_interrupt(int line){
	for(int i = 0; i < N_DEV_PER_IL; i++){
		if (Check_pending_interrupt(line, i)){

			//get the address of the device
			dtpreg_t *dev = (dtpreg_t*)DEV_REG_ADDR(line, i);

			//set the result on the process descriptor
			Set_IO(&dev_semaphores[line][i], dev->status);

			//release the process to be added to the ready queue
			SYSCALL_VERHOGEN(&dev_semaphores[line][i]);

			//acknowledge the interrupt
			dev->command = ACK_DEVICE;
		}
	}
}

void IT_interrupt(){

}

void PLT_interrupt(){
	//ackownledge the PLT interrupt by loading the timer with a new value

	setTIMER()

	//copy the processor state at the time into current pcb

	proc_curr->p_s = *((state_t*)BIOSDATAPAGE);

	//place the process in readyqueue

	insertProcQ(&readyQ, proc_curr);

	//call the scheduler
	Scheduler();
}

void Terminal_interrupt(){
	
}