
/*
TLB-Refill
    Eccezione che viene sollevata quando nel TLB non 
    viene trovata una entry valida.
    Non e’ da gestire in questa fase; potete popolare la 
    relativa parte del Pass Up Vector con la funzione 
    uTLB_RefillHandler() che troverete nel file p2test.c

*/

/*
ECCEZIONI
    le eccezioni bloccano il processo corrente per fare altro
    tipi di eccezioni:     il numero dell'eccezione è salvato nel registro Cause nel campo .ExcCode quindi Cause.ExcCode
    0 Interrupts: provocati da device
    8 system call
    1-3 eccezioni TLB: eccezioni che riguardano il memory management unit MMU
    4-7, 9-12 eccezioni Program Trap: Bus Error


PROCESSO CORRENTE IN USER MODE O KERNEL MODE
    per capire se il processo corrente era in Kernel mode o User mode si vede lo Status Register in Saved Exception State, in particolare
    si vede la versione precedente del bit KU (KUp) poiché il circuito di gestione delle eccezioni del processore avrà eseguito uno stack push sulla KU

*/
/*
GESTIONE DELLE SYSTEM CALL
    una eccezione di tipo system call arriva quando SYSCALL istruzione assembly avviene
    bisogna vedere i valori presenti nei registri a0, a1, a2, a3, a0 contiene il numero della system call e gli altri registri sono i parametri della SYSCALL
    il valore di ritorno della SYSCALL viene messo nel registro v0 
    In particolare se il processo era in Kernel Mode e in a0 c'è un valore fra 1 e 10 allora il Kernel deve fare uno di questi servizi:

    se vengono chiamate queste system call in user mode il Kernel simula una Program Trap exception, This is done by setting
    Cause.ExcCode in the stored exception state to RI (Reserved Instruction), and calling one’s Program Trap exception handler.
    lo stato di eccezione salvato(saved exception state) (per il processore 0) viene memorizzato all'inizio della pagina dati del BIOS (0x0FFFF000).

    RETURN DA UNA SYSTEM CALL
        si mette nel PC l'indirizzo dell'istruzione assembly della System Call + 4( 4 (ovvero il µMPS3 wordsize, questo serve a non andare in un loop
        infinito di chiamate a system call), non bisogna modificare t9 in questo caso     
    

    
     Exceptions related constants   si trovano nella parte con le strutture dati context_t e support_t
        #define PGFAULTEXCEPT 0
        #define GENERALEXCEPT 1

    SYSTEM CALL >= 11
        se Cuse.ExcCode è settato su 8(è una system call) e a0 è >= 11 allora il gestore delle eccezioni deve chiamare una operazione Pass Up or Die usando 
        GENERALEXCEPT index value
    
    Program Trap Exception Handling
        avviene quando il processo corrente performa delle azioni illegali o indefinite, l'eccezione program Trap viene gestita dal gestore delle eccezioni come una 
        operazione Pass Up or Die usando GENERALEXCEPT index value
    
    TLB Exception Handling
        una eccezione TLB avviene quando µMPS3 fallisce a tradurre un indirizzo logico in indirizzo fisico, viene gestita come una Pass Up or Die con PGFAULTEXCEPT index value
    */

/*
PASS UP VECTOR

    Nell’evento di un’eccezione uMPS3 salva lo stato 
    del processore in una specifica locazione di 
    memoria (0x0FFFF000) e carica PC e SP che trova nel Pass Up Vector


*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H


#include "pandos_arch.h"
#include "pandos_const.h"
#include "pandos_cp0.h"
#include "pandos_libumps.h"
#include "pandos_types.h"
#include "types.h"

#include "ash.h"
#include "ns.h"

#include "interrupts.h"
#include "scheduler.h"

#include "const.h"
#include "devices.h"
#include "state.h"



/*
 * EXCEPTIONS
 */

/* dispatcher for all kinds of exceptions. */
extern void Exception_handler();


/* system calls handler */
extern void SYSCALL_handler();


/* pass up exception handling to process' support struct if present,
 * otherwise kill the process.
 */
extern void PassUpOrDie(int excpt_type);


/* program trap handler */
#define Prg_Trap_handler() PassUpOrDie(GENERALEXCEPT)

/* TLB exceptions handler */
#define TLB_handler() PassUpOrDie(PGFAULTEXCEPT)


/*	TLB refill exceptions handler. 
	This function is not defined in this module,
	this it must be imported from somewhere else (test file).
*/
extern void uTLB_RefillHandler();


/*
 * SYSTEM CALLS
 */


/*SYSTEM CALL 1-10*/

/* Create a process, by allocating a pcb;
 * also initializing the fields given as parameters, and the others (including NULL parameters) to 0/NULL.
 * Return -1 on failure.
 */
extern void SYSCALL_CREATEPROCESS (state_t *statep, support_t * supportp, struct nsd_t *ns);

/*	Terminate the process to which the pid is associated, along with its children.
	If pid is 0, the current process is terminated (with its children).
*/
extern void SYSCALL_TERMINATEPROCESS (int pid);

/* perform a P on a semaphore.
*/
extern void SYSCALL_PASSEREN (int *semaddr);

/* perfirm a V on a semaphore.
*/
extern void SYSCALL_VERHOGEN (int *semaddr, int interrupt);

/* Start an IO request.
*/
extern void SYSCALL_DOIO (int *cmdAddr, int *cmdValues);

/*	Return the CPU time used by the calling process.
*/
extern void SYSCALL_GETCPUTIME ();

/* Perform a P on the pseudo-clock semaphore.
 * Thus, the calling process will remain blocked, waiting for a V from the Interval Timer.
 */
extern void SYSCALL_WAITCLOCK ();

/* get a pointer to current process' support struct (or NULL).
*/
extern void SYSCALL_GET_SUPPORT_DATA ();

/* if parent isn't TRUE, return process's pid;
 * else return parent's pid only if it shares process's pid namespace,
 * otherwise 0.
 */
extern void SYSCALL_GETPID (int parent);

/*	Load the `children` array with up to `size` children of the current process
	which are in its same namaspace; return the total number of children.
*/
extern void SYSCALL_GETCHILDREN (int *children, int size);


#endif /* EXCEPTIONS_H */
