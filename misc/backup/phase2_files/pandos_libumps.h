/*
 * External declarations for uMPS library module.
 */

#ifndef PANDOS_LIBUMBS_H_INCLUDED
#define PANDOS_LIBUMBS_H_INCLUDED

/*
 * "Forward declaration" hack!!
 * Many functions in this module accept a pointer to a cpu state
 * (STATE_PTR) structure. We cannot just forward declare that because
 * state_t was commonly defined by clients as an anonymous struct
 * typedef.
 */
#define STATE_PTR void*

/* Functions valid in user mode
 */


/* Only some of CP0 register are R/W: handling requires care.
 * All functions return the value in register after write
 */


extern unsigned int setENTRYLO(unsigned int entry);

extern unsigned int setENTRYHI(unsigned int entry);


/* This function allows a current process to change its operating mode,
 * turning on/off interrupt masks, turning on user mode, and at the same time
 * changing the location of execution.
 * It is available only in kernel mode, thru a BIOS routine
 * (otherwise it causes a break).
 * It updates processor status, PC and stack pointer _completely_,
 * in one atomic operation.
 * It has no meaningful return value.
 */

extern unsigned int LDCXT(unsigned int stackPtr, unsigned int status, unsigned int pc);


/* these functions produce a program trap if executed in user mode
 * without CPU0 bit _set_
 */

extern void TLBWR(void);


/* This function may be used to restart an interrupted/blocked process,
 * reloading it from a physical address passed as argument.
 * It is available only in kernel mode, thru a BIOS routine
 * (otherwise it causes a break).
 * It updates processor status _completely_, in one atomic operation.
 * It has no meaningful return value: $2 (v0) register is used for
 * BIOS call, but it is reloaded too.
 * Remember that it is programmer's task to increment PC where
 * needed (e.g. syscall handling)
 */

extern unsigned int LDST(STATE_PTR statep);


#endif /* PANDOS_LIBUMBS_H_INCLUDED */