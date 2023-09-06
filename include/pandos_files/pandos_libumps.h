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


/* All these functions access CP0 registers.
 * Access to CP0 registers is always possible in kernel mode, or in user
 * mode with CPU 0 bit _set_ in STATUS register
 */

extern unsigned int getINDEX(void);

extern unsigned int getRANDOM(void);

extern unsigned int getENTRYLO(void);

extern unsigned int getBADVADDR(void);

extern unsigned int getENTRYHI(void);

extern unsigned int getSTATUS(void);

extern unsigned int getCAUSE(void);

extern unsigned int getEPC(void);

extern unsigned int getPRID(void);

extern unsigned int getTIMER(void);


/* Only some of CP0 register are R/W: handling requires care.
 * All functions return the value in register after write
 */

extern unsigned int setINDEX(unsigned int index);

extern unsigned int setENTRYLO(unsigned int entry);

extern unsigned int setENTRYHI(unsigned int entry);

extern unsigned int setSTATUS(unsigned int entry);

extern unsigned int setCAUSE(unsigned int cause);

extern unsigned int setTIMER(unsigned int timer);


/* these functions produce a program trap if executed in user mode
 * without CPU0 bit _set_
 */

extern void TLBWR(void);

extern void WAIT(void);


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


/* This function stops the system printing a warning message on terminal 0
 */

extern void PANIC(void);


/* This function halts the system printing a regular shutdown message on
 * terminal 0
 */

extern void HALT(void);


#endif /* PANDOS_LIBUMBS_H_INCLUDED */