#include "include/phase1_files/types.h"
#include "include/phase1_files/pandos_types.h"

/* process control block type */
typedef struct pcb_t {
	/* process queue fields */
	struct pcb_t 	*p_next,		/* pointer to next entry */
					*p_prev,		/* pointer to prev entry */
					
	/* process tree fields */
					*p_prnt,		/* pointer to parent */
					*p_child,		/* pointer to 1st child */
					*p_sib;			/* pointer to sibling */

	/* process status information */
	state_t			p_s;			/* processor state */
	cpu_t			p_time;			/* cpu time used by proc */
	int 			*p_semAdd;		/* pointer to sema4 on */
									/* which process blocked */

	/* support layer information */
	support_t *p_supportStruct;		/* ptr to support struct */

	/* namespace layer information */
	nsd_t *namespaces[NS_TYPE_MAX];	/* Namespace pointer per type */
} pcb_t;


void freePcb(pcb_t *p);
	/* Insert the element pointed to by p onto the pcbFree list.
	*/

pcb_t *allocPcb();
	/* Return NULL if the pcbFree list is empty. Otherwise, remove an element from the pcbFree list, provide initial values
	for ALL of the pcbs fields (i.e. NULL and/or 0) and then
	return a pointer to the removed element. pcbs get reused, so
	it is important that no previous value persist in a pcb when
	it gets reallocated. */

initPcbs();
	/* Initialize the pcbFree list to contain all the elements of
	the static array of MAXPROC pcbs. This method will be
	called only once during data structure initialization. */
	
	/*A static array of MAXPROC pcbs will be declared in initPcbs().
	Furthermore, this method will insert each of the
	MAXPROC pcbs onto the pcbFree list.	*/

pcb_t *mkEmptyProcQ();
	/* This method is used to initialize a variable to be tail pointer
	to a process queue.
	Return a pointer to the tail of an empty process queue; i.e. NULL.
	*/

int emptyProcQ(pcb_t *tp);
	/* Return TRUE if the queue whose tail is pointed to by tp is
	empty. Return FALSE otherwise. */

insertProcQ(pcb_t **tp, pcb_t *p);
	/* Insert the pcb pointed to by p into the process queue whose tailpointer is pointed to by tp. Note the double indirection through
	tp to allow for the possible updating of the tail pointer as well.
	*/

pcb_t *removeProcQ(pcb_t **tp);
	/* Remove the first (i.e. head) element from the process queue
	whose tail-pointer is pointed to by tp. Return NULL if the process queue was initially empty; otherwise return the pointer to
	the removed element. Update the process queue’s tail pointer if
	necessary. */
	
pcb_t *outProcQ(pcb_t **tp, pcb_t *p);
	/* Remove the pcb pointed to by p from the process queue whose
	tail-pointer is pointed to by tp. Update the process queue’s tail
	pointer if necessary. If the desired entry is not in the indicated
	queue (an error condition), return NULL; otherwise, return p.
	Note that p can point to any element of the process queue. */

pcb_t *headProcQ(pcb_t *tp);
	/* Return a pointer to the first pcb from the process queue whose
	tail is pointed to by tp. Do not remove this pcbfrom the process
	queue. Return NULL if the process queue is empty. */

int emptyChild(pcb_t *p);
	/* Return TRUE if the pcb pointed to by p has no children.
	Return FALSE otherwise. */

insertChild(pcb_t *prnt, pcb_t *p);
	/* Make the pcb pointed to by p a child of the pcb pointed to by
	prnt. */

pcb_t *removeChild(pcb_t *p);
	/* Make the first child of the pcb pointed to by p no longer a
	child of p. Return NULL if initially there were no children of p.
	Otherwise, return a pointer to this removed first child pcb. */
	
pcb_t *outChild(pcb_t *p);
	/* Make the pcb pointed to by p no longer the child of its parent.
	If the pcb pointed to by p has no parent, return NULL; otherwise,
	return p. Note that the element pointed to by p need not be the
	first child of its parent. */