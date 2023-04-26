#ifndef HELPER_H
#define HELPER_H



#include "initial.h"

#define TLB_EXCEPTION(x)(x>=EXC_MOD && x<=EXC_TLBS)
#define TRAP_EXCEPTION(x)((x>=EXC_ADEL && x<=EXC_DBE) || (x>=EXC_BP && x<=EXC_OV))


/*copio lo stato sorgente nello stato destinazione*/
extern void cp_state(state_t* src_state, state_t* dst_state);
extern state_t get_saved_exceptions_state();
/*dice se il processo corrente è in user mode*/
extern int is_UM();



#endif