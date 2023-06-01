#include "helper/exceptions_help.h"


/* farla inline / macro
per coerenza potrebbe avere senso UPPERCASE,
come le macro CAUSE_*
*/
void state_copy(state_t* src_state, state_t dst_state){
/* src e dst già sono chiari come nomi, ma almeno più corti */
/* i tab rendrebbero tutto più leggibile:
	dst_state.entry_hi	= src_state->entry_hi;
	dst_state.cause		= src_state->cause;
	dst_state.status	= src_state->status;
	...
*/
	dst_state.entry_hi = src_state->entry_hi;
	dst_state.cause	= src_state->cause;
	dst_state.status = src_state->status;
	dst_state.pc_epc = src_state->pc_epc;
	dst_state.hi = src_state->hi;
	dst_state.lo = src_state->lo;
	/* non servono le graffe {} per i for a 1 riga */
	for(int i=0;i<STATE_GPR_LEN;i++){dst_state.gpr[i]=src_state->gpr[i];}
	/* qualche spazio non farebbe schifo */
}

int is_UM(){
	return (SAVED_EXCEPTIONS_STATE->status==BIT_USER);
}
/* MACRO, in h */