#ifndef ASH_HELP_H
#define ASH_HELP_H

#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "linux_hashtable.h"
#include "pcb.h"

/*dice se la coda dei processi bloccati relativa al semaforo è vuota*/
extern int sem_queue_is_empty(int *semAdd);


/*
  Remove the pcb from the semahore's queue on which it's blocked and return the sem.
  If there remains no more pcb's blocked on it, set the semaphore as free.
  Return NULL if p is not in its semaphore's queue.

extern semd_t * my_outBlocked(pcb_t *p);
*/
#endif /* ASH_HELP_H */