#ifndef EXCEPTIONS_HELP_H
#define EXCEPTIONS_HELP_H


#include "pandos_bios.h"
#include "pandos_types2.h"

#include "pandos_cp0.h"
#include "const.h"



#define TLB_EXCEPTION(x) (x>=EXC_MOD && x<=EXC_TLBS)
#define TRAP_EXCEPTION(x) (x>=EXC_ADEL && x<=EXC_DBE) || (x>=EXC_BP && x<=EXC_OV)

#define IS_SYSCALL(x) ((x>=CREATEPROCESS) && (x<=GET_TOD))
//ritorna il saved exceptions state
#define SAVED_EXCEPTIONS_STATE  ((state_t *)BIOS_DATA_PAGE_BASE);

/*copio lo stato sorgente nello stato destinazione*/
extern void state_copy(state_t* src_state, state_t dst_state);
/*dice se il processo corrente è in user mode*/
extern int is_UM();



#endif /* EXCEPTIONS_HELP_H */