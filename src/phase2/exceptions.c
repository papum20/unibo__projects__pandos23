/*
exceptions.c
This module implements the TLB, Program Trap, and
SYSCALL exception handlers. Furthermore, this module will contain
the provided skeleton TLB-Refill event handler (e.g. uTLB_RefillHandler).
*/



void uTLB_RefillHandler () {
	setENTRYHI(0x80000000);
	setENTRYLO(0x00000000);
	TLBWR();
	LDST ((state_PTR) 0x0FFFF000);
}