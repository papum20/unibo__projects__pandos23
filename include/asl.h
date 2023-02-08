#include "include/pcb.h"

/* semaphore descriptor type */
typedef struct semd_t {
	struct hash_table_entry s_link; 	/* ASH reference */
	struct semd_t			*s_freelink;/* next element on the */
										/* free semaphore list */
	int						*s_semAdd; 	/* semaphore pointer */
	pcb_t 					*s_procQ; 	/* tail pointer to a */
										/* process queue */
} semd_t;


int insertBlocked(int *semAdd, pcb_t *p);
	/* Insert the pcb pointed to by p at the tail of the process queue
	associated with the semaphore whose physical address is semAdd
	and set the semaphore address of p to semAdd. If the semaphore
	is currently not active (i.e. there is no descriptor for it in the
	ASH), allocate a new descriptor from the semdFree list, insert it
	in the ASH, initialize all of the fields (i.e. set s semAdd to semAdd,
	and s procq to mkEmptyProcQ()), and proceed as above. If a new
	semaphore descriptor needs to be allocated and the semdFree list
	is empty, return TRUE. In all other cases return FALSE. */

pcb_t *removeBlocked(int *semAdd);
	/* Search the ASH for a descriptor of this semaphore. If none is
	found, return NULL; otherwise, remove the first (i.e. head) pcb
	from the process queue of the found semaphore descriptor and
	return a pointer to it. If the process queue for this semaphore
	becomes empty (emptyProcQ(s procq) is TRUE), remove the
	semaphore descriptor from the ASH and return it to the semdFree
	list. */

pcb_t *outBlocked(pcb_t *p);
	/* Remove the pcb pointed to by p from the process queue associated with p’s semaphore (p→ p semAdd) on the ASH. If pcb
	pointed to by p does not appear in the process queue associated
	with p’s semaphore, which is an error condition, return NULL;
	otherwise, return p. */

pcb_t *headBlocked(int *semAdd);
	/* Return a pointer to the pcb that is at the head of the process
	queue associated with the semaphore semAdd. Return NULL if
	semAdd is not found on the ASH or if the process queue associated
	with semAdd is empty. */

initASH();
	/* Initialize the semdFree list to contain all the elements of the
	array
	static semd t semdTable[MAXPROC]
	This method will be only called once during data structure initialization. */