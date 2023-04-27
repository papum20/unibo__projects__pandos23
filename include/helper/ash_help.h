#ifndef ASH_HELP_H
#define ASH_HELP_H

#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "linux_hashtable.h"
#include "pcb.h"

/*dice se la coda dei processi bloccati relativa al semaforo è vuota*/
extern int sem_queue_is_empty(int *semAdd);

#endif /* ASH_HELP_H */