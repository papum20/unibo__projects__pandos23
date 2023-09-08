
#ifndef INITIAL_HELP_H
#define INITIAL_HELP_H


#include "state.h"

#include "pandos_arch.h"
#include "pandos_types.h"



/*
 * IMPLEMENTATION CONSTANTS
 */

/*	Init value for the interval timer */
#define INTERVAL_TIMER_INIT 100000


/* return the initialization value for the first process */
#define _INIT_FIRST_PROC_STATE(state) (						\
	STATUS_SET_KUp(											\
		STATUS_SET_IEp_ALL(									\
			STATUS_SET_TE(									\
				STATUS_SET_CU0(								\
					state, BIT_ENABLED						\
				),											\
				BIT_ENABLED									\
			),												\
			BIT_ENABLED										\
		), BIT_KERNEL										\
))




#endif /* INITIAL_HELP_H */