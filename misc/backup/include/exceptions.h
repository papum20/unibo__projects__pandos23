

/*
TEMPORARY NAME!!
*/

/*
indirizzi di memoria che mi servono 
    pandos_const.h

        #define PASSUPVECTOR 0x0FFFF900

        #define TIMESLICE  5000  length of proc's time slice	 forse mi serve

         Cause register constants 
            #define SYSEXCEPTION   8
            #define BREAKEXCEPTION 9
*/

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

GESTIONE DELLE ECCEZIONI
    per gestire le eccezioni si usa uno switch che in base al numero in Cause.ExcCode fa codici diversi
    Le eccezioni non possono essere annidate.

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
        SYSCALL 1: Create_Process
        SYSCALL 2: Terminate_Process
        SYSCALL 3: Passeren  (La P dei semafori)
        SYSCALL 4: Verhogen  (La V dei semafori)
        SYSCALL 5: Do_IO
        SYSCALL 6: Get_CPU_Time
        SYSCALL 7: Wait_For_Clock
        SYSCALL 8: Get_SUPPORT_Data
        SYSCALL 9:  Get_Process_ID
        SYSCALL 10: Get_children
    se vengono chiamate queste system call in user mode il Kernel simula una Program Trap exception, This is done by setting
    Cause.ExcCode in the stored exception state to RI (Reserved Instruction), and calling one’s Program Trap exception handler.
    lo stato di eccezione salvato(saved exception state) (per il processore 0) viene memorizzato all'inizio della pagina dati del BIOS (0x0FFFF000).

    RETURN DA UNA SYSTEM CALL
        si mette nel PC l'indirizzo dell'istruzione assembly della System Call + 4( 4 (ovvero il µMPS3 wordsize, questo serve a non andare in un loop
        infinito di chiamate a system call), non bisogna modificare t9 in questo caso     
    
    SYSTEM CALL CHE BLOCCANO
        in questo caso bisogna fare vari step:
        1)incrementare il PC come detto sopra
        2)lo stato del processore salvato che si trova all'inizio del BIOS Data Page 0x0FFF.F000 deve essere copiato nel campo p_s del pcb corrente
        3)aggiorna il Accumulated CPU Time(sarebbe quanto il processo ha occupato il tempo della CPU) del processo corrente quindi aggiorna
          il campo p_time di pcb aggiungendo il tempo usato dalla CPU durante il time slice, per tenere traccia del tempo usiamo il TOD clock presente in µMPS3 
        4)il processo viene bloccato in ASL(insertBlocked)(si incrementa Soft-block Count e penso che poi il pcb debba essere tolto da qualche coda) facendo transitare il suo stato da running a blocked
        5)chiama lo scheduler
    
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
    esso è una struttura che 
    dovete popolare all’indirizzo 0x0FFFF900(PASSUPVECTOR)

    The Pass Up Vector is where the BIOS finds the address of the Nucleus
    functions to pass control to for both TLB-Refill events and all other
    exceptions.

    Nell’evento di un’eccezione uMPS3 salva lo stato 
    del processore in una specifica locazione di 
    memoria (0x0FFFF000) e carica PC e SP che trova nel Pass Up Vector

    Il Pass Up Vector distingue tra TLB-Refill e tutte 
    le altre eccezioni (per distinguere ulteriormente 
    si veda il registro Cause).
____________________________________________________________________________

    all'avvio, il Kernel avrà popolato il Processor 0 Pass Up Vector con l'indirizzo del gestore delle eccezioni del Nucleus (fooBar)
    e l'indirizzo della pagina dello stack del Nucleus (0x2000.1000).
    Pertanto, se il Pass Up Vector è stato inizializzato correttamente, 
    fooBar verrà chiamato (con un nuovo stack) dopo ogni singola eccezione, esclusi gli eventi TLBRefill.
*/

/*
PARTE DI DANIELE, inizializzazione di roba che serve a me
    Set the Nucleus TLB-Refill event handler address to

    xxx->tlb_refll_handler = (memaddr) uTLB_RefillHandler;

    void uTLB_RefillHandler () {
        setENTRYHI(0x80000000);
        setENTRYLO(0x00000000);
        TLBWR();
        LDST ((state_PTR) 0x0FFFF000);
    }

    e memaddr, in pandos_types.h è unsigned int.
    Since address translation is not implemented until the Support
    Level, uTLB_RefillHandler is a place holder function whose code
    is provided. [Section 3.3] This code will then be replaced when
    the Support Level is implemented.

    • Set the Stack Pointer for the Nucleus TLB-Refill event handler to
    the top of the Nucleus stack page: 0x2000.1000. Stacks in µMPS3
    grow down.

    • Set the Nucleus exception handler address to the address of your
    Level 3 Nucleus function (e.g. foobar) that is to be the entry
    point for exception (and interrupt) handling [Section 3.4]:

    xxx->exception_handler = (memaddr) fooBar;

    • Set the Stack pointer for the Nucleus exception handler to the top
    of the Nucleus stack page: 0x2000.1000.
*/

/*
CURIOSITA'
    LDST sarebbe il caricamento dello stato del processore
*/

/*
il pass up vector all'interno ha 4 campi unsigned int che sarebbero gli indirizzi di memoria di SP e della funzione exception handler
quando il p2test che sarebbe un utente chiama ad esempio la funzione SYSCALL quei parametri il bios li mette nei registri a0-a3
e poi grazie al pass up vector chiama la funzione exception handler
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
#include "ash_help.h"
#include "exceptions_help.h"
#include "pcb_help.h"


/*
* ECCEZIONI
*/
	/*gestisce le varie eccezioni*/
	/*gestisce le eccezioni (di ogni tipo) */
	extern void Exception_handler();


	/*gestisce le system call*/
	extern void SYSCALL_handler();


	/*per le eccezioni 11 o superiore o eccezioni Program Trap o Tlb, si esegue una Pass Up or Die*/
	extern void PassUpOrDie(int excpt_type);


	/*gestisce le eccezioni Program Trap*/
	extern inline void Prg_Trap_handler(){
		PassUpOrDie(GENERALEXCEPT);
	}


	/*gestisce le eccezioni TLB*/
	extern inline void TLB_handler(){
		PassUpOrDie(PGFAULTEXCEPT);
	}


	extern void uTLB_RefillHandler ();


/*
 * SYSTEM CALL
 */
	/*operazioni da fare nel return di una system call che blocca capitolo 3.5.13*/
	/* cosa fa? */
	extern void BlockingSyscall(int *semaddr);


		/*SYSTEM CALL 1-10*/
			/*
			SYSCALL_CREATEPROCESS
			crea un nuovo processo  diventando figlio del processo chiamante, se tutto va bene questa system call ritorna il pid
			del nuovo processo creato, se invece la lista dei pcb free è vuota allora ritorna -1
			statep è il p_s del nuovo processo creato, supportp è la struttura di supporto del nuovo processo e ns il suo namespace
			*/
			extern void SYSCALL_CREATEPROCESS(state_t *statep, support_t * supportp, struct nsd_t *ns);
			/*
			SYSCALL_TERMINATEPROCESS
			termina il processo con quel rispettivo pid e i suoi figli, se pid=0 allora termina il processo corrente e i figli
			*/
			extern void SYSCALL_TERMINATEPROCESS (int pid);
			/*
			SYSCALL_PASSEREN
			P su un semaforo
			*/
			extern void SYSCALL_PASSEREN (int *semaddr);
			/*
			SYSCALL_VERHOGEN
			V su un semaforo
			*/
			extern void SYSCALL_VERHOGEN (int *semaddr);
			/*
			SYSCALL_DOIO
			*/
			extern void SYSCALL_DOIO (int *cmdAddr, int *cmdValues);
			/*
			SYSCALL_GETCPUTIME
			ritorna il tempo che il processo ha impiegato la CPU + tempo usato dalla CPU durante il current quantum/time slice
			*/
			extern void SYSCALL_GETCPUTIME ();
			/*
			SYSCALL_WAITCLOCK
			Performa una P sullo pseudoclock(Interval Timer), questa system call blocca sempre 
			*/
			extern void SYSCALL_WAITCLOCK();
			/*
			SYSCALL_GET_SUPPORT_DATA
			ritorna il puntatore alla struttura di supporto del processo corrente
			*/
			extern void SYSCALL_GET_SUPPORT_DATA();
			/*
			SYSCALL_GETPID
			se parent = TRUE 
				se il processo padre del processo corrente è nello stesso PID namespace del processo corrente allora ritorna il PID del padre
				invece se non è nello stesso namespace ritorna 0
			se parente = FALSE
				ritorna il pid del processo corrente
			*/
			extern void SYSCALL_GETPID( int parent);
			/*
			SYSCALL_GETCHILDREN
			ritorna il numero di figli nello stesso namespace e mette nell'array children i loro pid
			*/
			extern void SYSCALL_GETCHILDREN(int *children, int size);


#endif /* EXCEPTIONS_H */
