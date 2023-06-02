#include "interrupts.h" 



void Interrupt_handler() {
	/*
	Non-Timer Interrupts:
	1. Calculate the address for this device’s device register. [Section ??-pops]

	2. Save off the status code from the device’s device register.

	3. Acknowledge the outstanding interrupt. This is accomplished by writ-
	ing the acknowledge command code in the interrupting device’s device
	register. Alternatively, writing a new command in the interrupting
	device’s device register will also acknowledge the interrupt.
	*/

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

