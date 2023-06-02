
/****************************************************************************
 *
 * This header file contains utilities for managing system clocks.
 *
 ****************************************************************************/



#ifndef CLOCKS_H
#define CLOCKS_H


#include "pandos_const.h"


/* current TOD clock value */
#define TODCK ( (*((cpu_t *) TODLOADDR)) / (*((cpu_t *) TIMESCALEADDR)) )

/* CPU time used by a process */
#define CPU_TIME_USED(proc) (proc->p_time + TODCK)

/**/
#define PROC_TIME_UPDATE(proc) (proc->p_time += TODCK)


#endif /* CLOCKS_H */