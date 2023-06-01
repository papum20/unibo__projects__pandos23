#include "helper.h"


void cp_state(state_t* src_state, state_t dst_state){
	dst_state.entry_hi = src_state->entry_hi;
	dst_state.cause = src_state->cause;
	dst_state.status = src_state->status;
	dst_state.pc_epc = src_state->pc_epc;
	dst_state.hi = src_state->hi;
	dst_state.lo = src_state->lo;
	for(int i=0;i<29;i++){dst_state.gpr[i]=src_state->gpr[i];}
}
state_t * get_saved_exceptions_state(){
	state_t * saved_exceptions_state;
	saved_exceptions_state =  BIOS_DATA_PAGE_BASE;
	return  saved_exceptions_state;
}
int is_UM(){
	state_t * saved_exceptions_state = get_saved_exceptions_state();
	return (saved_exceptions_state->status==BIT_USER);
}


#define SAVED_EXCEPTION_STATE ((state_t*) BIOS_DATA_PAGE_BASE)
