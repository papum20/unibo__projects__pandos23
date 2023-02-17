#ifndef ASH_H
#define ASH_H

#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "linux_hashtable.h"
#include "pcb.h"



/**
 * Initialize the semdFree_h list to contain MAXPROC elements,
 * and initialize the hash table.
 * This function should be called once during data structure initialization.
*/
extern void initASH();



/* ASH MANAGEMENT */

/**
 * Insert the pcb p in the queue of pcb's blocked on the semaphore with key semAdd.
 * If the semaphore is currently not active (i.e. there is no descriptor for it in the
 * ASH), allocate a new one and insert it in the ASH.
 * If a new semaphore descriptor needs to be allocated and the semdFree list
 * is empty, return TRUE.
 * In all other cases return FALSE.
*/
extern int insertBlocked(int *semAdd, pcb_t *p);

/**
 * Remove and return the first pcb blocked on the semaphore with key semAdd.
 * If there remains no more pcb's blocked on it, set the semaphore as free.
 * Return NULL if no process is blocked on it.
*/
extern pcb_t *removeBlocked(int *semAdd);

/**
 * Remove and return the pcb p from the semahore's queue on which it's blocked.
 * If there remains no more pcb's blocked on it, set the semaphore as free.
 * Return NULL if p is not in its semaphore's queue.
*/
extern pcb_t *outBlocked(pcb_t *p);

/**
 * Return, without removing, the first pcb blocked on the semaphore with key semAdd.
 * Return NULL if no pcb is blocked on it.
*/
extern pcb_t *headBlocked(int *semAdd);



#endif