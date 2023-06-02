#ifndef EXCEPTIONS_HELP_H
#define EXCEPTIONS_HELP_H


#include "pandos_bios.h"
#include "pandos_cp0.h"
#include "pandos_types.h"

#include "const.h"

/*
* ECCEZIONI
*/
	/* usa i comment con /*,
		perché 1. vecchie versioni di c non supportano //,
		2. per coerenza (quasi tutti sono cosi)
		*/
	// saved exceptions state
	#define SAVED_EXCEPTIONS_STATE  ((state_t *)BIOS_DATA_PAGE_BASE)


	#define __CAUSE_EXCCODE_SETTED(cause, x) (cause & !CAUSE_EXCCODE_MASK) | (x << CAUSE_EXCCODE_BIT)


	//setta il registro exe_code
	static inline CAUSE_SET_EXCCODE(state_t state, unsigned int code) {
		state.cause = __CAUSE_EXCCODE_SETTED(state.cause, code);
	}


	//dice se si è sollevata una eccezione TLB
	/* no: dice se l'exe_code contiene una tlb exception */
	#define IS_TLB_EXCEPTION(exe_code) (exe_code>=EXC_MOD && exe_code<=EXC_TLBS)


	//dice se si è sollevata una eccezione TRAP
	/* no: vd su */
	#define IS_TRAP_EXCEPTION(exe_code) (exe_code>=EXC_ADEL && exe_code<=EXC_DBE) || (exe_code>=EXC_BP && exe_code<=EXC_OV)


/*
* SYSTEM CALL
*/
	/*
		dice se nel registro a0 è presente un valore fra 1 e 10 quindi è stata chiamata una fra le varie SYSTEM CALL, dando per 
		presupposto che già nel registo exe_code ci sia il valore 8
	*/
	#define IS_SYSCALL(reg_a0) ((reg_a0>=CREATEPROCESS) && (reg_a0<=GET_TOD))


	/*
		macro che gestisce dove mettere il valore di ritorno delle system call che ritornano qualcosa
		essa mette nel registro v0 del current process il pid di un processo passato come parametro
	*/
	/*## 1.lo vedo che è una macro, non serve che scrivi che è una macro */
	/*## 2.l'italiano: 1. la punteggiatura, 2 essa (vd scheduler..)*/
	/*## 3.chi è current_proc? */
	/*## chiamata cosi è poco chiaro il suo uso (cioè perché cambi v0). piuttosto, visto il codice, farei
	RETURN_SYSCALL(val): ritorna con valore
	RETURN_SYSCALL_VOID(): ritorna senza valore
	e così chiami ogni volta solo una return
	*/
	#define UPDATE_REGV0(value) (current_proc->p_s.reg_v0 = (memaddr)value)


	/*
		funzione che gestisce il ritorno di una system call
		aggiorna il saved exception state aumentando program counter di una word per evitare loop e aggiornando il CPU Time del processo corrente
	*/



	/*## usa la punteggiatura, esiste... */
	/*## lo vedo che è una funzione */
	/*## non mettere spazi tra commenti e funzioni, o non si capisce a cosa si riferiscono*/
	inline void RETURN_SYSCALL(){
		SAVED_EXCEPTION_STATE->pc_epc += WORDLEN;
		state_copy(SAVED_EXCEPTION_STATE, current_proc->p_s);
	}


/*copio lo stato sorgente nello stato destinazione*/
/*## non copi tu, lo fa il programma */
extern void state_copy(state_t* src_state, state_t dst_state);


/*dice se il processo corrente è in user mode*/
extern int is_UM();


#define SYSCALL(a0, a1, a2, a3)	\
		switch(a0){				\
			case CREATEPROCESS:							\
				SYSCALL_CREATEPROCESS((state_t *)a1, (support_t *) a2, (struct nsd_t *) a3);	\
				break;									\
			case TERMPROCESS:							\
				SYSCALL_TERMINATEPROCESS (a1);			\
				break;									\
			case PASSEREN:								\
				SYSCALL_PASSEREN((int *)a1);			\
				break;									\
			case VERHOGEN:								\
				SYSCALL_VERHOGEN((int *)a1);			\
				break;									\
			case IOWAIT:								\
				SYSCALL_DOIO((int *)a1,(int *)a2);		\	
				break;									\
			case GETTIME:								\
				SYSCALL_GETCPUTIME();					\
				break;									\
			case CLOCKWAIT:								\
				SYSCALL_WAITCLOCK();					\
				break;									\
			case GETSUPPORTPTR:							\
				SYSCALL_GET_SUPPORT_DATA();				\
				break;									\
			case TERMINATE:								\
				SYSCALL_GETPID(a1);						\
				break;									\
			case GET_TOD:								\
				SYSCALL_GETCHILDREN((int *)a1, a2);		\
		}	


#endif /* EXCEPTIONS_HELP_H */