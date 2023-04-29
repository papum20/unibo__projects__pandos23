#include "ash_help.h"

inline int sem_queue_is_empty(int *semAdd){														
	semd_t *sem = __hash_semaphore(semAdd);
	return emptyProcQ(&sem->s_procq) == true;	

}
