#ifndef EXCEPTIONS_HELP_H
#define EXCEPTIONS_HELP_H


#include "pandos_bios.h"
#include "pandos_cp0.h"
#include "pandos_types.h"

#include "const.h"

/*
* ECCEZIONI
*/
	// saved exceptions state
	#define SAVED_EXCEPTIONS_STATE  ((state_t *)BIOS_DATA_PAGE_BASE)


	#define __CAUSE_EXCCODE_SETTED(cause, x) (cause & !CAUSE_EXCCODE_MASK) | (x << CAUSE_EXCCODE_BIT)


	//setta il registro exe_code
	static inline CAUSE_SET_EXCCODE(state_t state, unsigned int code) {
		state.cause = __CAUSE_EXCCODE_SETTED(state.cause, code);
	}


	//dice se si è sollevata una eccezione TLB
	#define IS_TLB_EXCEPTION(exe_code) (exe_code>=EXC_MOD && exe_code<=EXC_TLBS)


	//dice se si è sollevata una eccezione TRAP
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


	#define UPDATE_REGV0(value) (current_proc->p_s.reg_v0 = (memaddr)value)


	/*
		funzione che gestisce il ritorno di una system call
		aggiorna il saved exception state aumentando program counter di una word per evitare loop e aggiornando il CPU Time del processo corrente
	*/



	inline void RETURN_SYSCALL(){
		SAVED_EXCEPTIONS_STATE->pc_epc += WORDLEN;
		state_copy(SAVED_EXCEPTIONS_STATE, current_proc->p_s);
	}


/*copio lo stato sorgente nello stato destinazione*/
extern void state_copy(state_t* src_state, state_t dst_state);


/*dice se il processo corrente è in user mode*/
extern int is_UM();


#endif /* EXCEPTIONS_HELP_H */