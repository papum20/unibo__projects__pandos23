#ifndef ASH_H
#define ASH_H

#include "phase1_files/pandos_const.h"
#include "phase1_files/pandos_types.h"
#include "phase1_files/types.h"
#include "phase1_files/hashtable.h"
#include "pcb.h"



/* Initialize the semdFree list to contain all the elements of the
array
static semd t semdTable[MAXPROC]
This method will be only called once during data structure initialization. 
*/
void initAsh();



/* Insert the pcb pointed to by p at the tail of the process queue
associated with the semaphore whose physical address is semAdd
and set the semaphore address of p to semAdd. If the semaphore
is currently not active (i.e. there is no descriptor for it in the
ASH), allocate a new descriptor from the semdFree list, insert it
in the ASH, initialize all of the fields (i.e. set s_key to semAdd,
and s_procq to an empty list), and proceed as above. If a new
semaphore descriptor needs to be allocated and the semdFree list
is empty, return TRUE. In all other cases return FALSE.
*/
int insertBlocked(int *semAdd, pcb_t *p);



/* Search the ASH for a descriptor of this semaphore. If none is
found, return NULL; otherwise, remove the first (i.e. head) pcb
from the process queue of the found semaphore descriptor and
return a pointer to it. If the process queue for this semaphore
becomes empty (lista_vuota is TRUE), remove the
semaphore descriptor from the ASH and return it to the semdFree
list. */
pcb_t* removeBlocked(int *semAdd);



/* Return a pointer to the pcb that is at the head of the process
queue associated with the semaphore semAdd. Return NULL if
semAdd is not found on the ASH or if the process queue associated
with semAdd is empty. */
pcb_t* headBlocked(int *semAdd);


/* Remove the pcb pointed to by p from the process queue associated with p’s semaphore (p→ p semAdd) on the ASH. If pcb
pointed to by p does not appear in the process queue associated
with p’s semaphore, which is an error condition, return NULL;
otherwise, return p. 
*/
pcb_t* outBlocked(pcb_t *p);


//useful function that iterates through a hashtable and returns
//the semaphore associated with the hash of the key
//if not present, it returns a pointer pointing to NULL
static inline semd_t* hash_semaphore(int* key);


#endif