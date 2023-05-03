#include "helper/exceptions_help.h"


void state_copy(state_t* src_state, state_t dst_state){
	dst_state.entry_hi = src_state->entry_hi;
	dst_state.cause = src_state->cause;
	dst_state.status = src_state->status;
	dst_state.pc_epc = src_state->pc_epc;
	dst_state.hi = src_state->hi;
	dst_state.lo = src_state->lo;
	for(int i=0;i<STATE_GPR_LEN;i++){dst_state.gpr[i]=src_state->gpr[i];}
}

int is_UM(){
	state_t * saved_exceptions_state = SAVED_EXCEPTIONS_STATE;
	return (saved_exceptions_state->status==BIT_USER);
}