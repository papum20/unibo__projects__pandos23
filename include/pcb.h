#include "phase1_files/pandos_const.h"
#include "phase1_files/pandos_types.h"
#include "phase1_files/types.h"
/*	pandos_const.h is included before list.h,
	as the former defines NULL while the latter requires it
	(this is just for the editor, as in execution it doesn't matter) */
#include "phase1_files/list.h"



void initPcbs();
	/* Initialize the pcbFree list to contain all the elements of
	the static array of MAXPROC pcbs. This method will be
	called only once during data structure initialization. */
	/*A static array of MAXPROC pcbs will be declared in initPcbs().
	Furthermore, this method will insert each of the
	MAXPROC pcbs onto the pcbFree list.	*/
	/* Inizializza la lista pcbFree in modo da
	contenere tutti gli elementi della
	pcbFree_table. Questo metodo deve
	essere chiamato una volta sola in fase di
	inizializzazione della struttura dati. */



//// ALLOCATION

void freePcb(pcb_t *p);
	/* Insert the element pointed to by p onto the pcbFree list.
	*/
	/* Inserisce il PCB puntato da p nella lista
	dei PCB liberi (pcbFree_h) */

pcb_t *allocPcb();
	/* Return NULL if the pcbFree list is empty. Otherwise, remove an element from the pcbFree list, provide initial values
	for ALL of the pcbs fields (i.e. NULL and/or 0) and then
	return a pointer to the removed element. pcbs get reused, so
	it is important that no previous value persist in a pcb when
	it gets reallocated. */
	/* Restituisce NULL se la pcbFree_h è vuota.
	Altrimenti rimuove un elemento dalla
	pcbFree, inizializza tutti i campi (NULL/0)
	e restituisce l’elemento rimosso */



//// LISTS

void mkEmptyProcQ(struct list_head *head);
	/* This method is used to initialize a variable to be tail pointer
	to a process queue.
	Return a pointer to the tail of an empty process queue; i.e. NULL.
	*/
	/* Crea una lista di PCB, inizializzandola
	come lista vuota */

int emptyProcQ(struct list_head *head);
	/* Return TRUE if the queue whose tail is pointed to by tp is
	empty. Return FALSE otherwise. */
	/* Restituisce TRUE se la lista puntata da
	head è vuota, FALSE altrimenti*/

void insertProcQ(struct list_head *head, pcb_t *p);
	/* Insert the pcb pointed to by p into the process queue whose tailpointer is pointed to by tp. Note the double indirection through
	tp to allow for the possible updating of the tail pointer as well.
	*/
	/* Inserisce l’elemento puntato da p nella
	coda dei processi puntata da head. */

pcb_t *headProcQ(struct list_head *head);
	/* Return a pointer to the first pcb from the process queue whose
	tail is pointed to by tp. Do not remove this pcbfrom the process
	queue. Return NULL if the process queue is empty. */
	/* Restituisce l’elemento di testa della coda dei processi da head, SENZA
	RIMUOVERLO. Ritorna NULL se la coda non ha elementi. */
	
pcb_t *removeProcQ(struct list_head* head);
	/* Remove the first (i.e. head) element from the process queue
	whose tail-pointer is pointed to by tp. Return NULL if the process queue was initially empty; otherwise return the pointer to
	the removed element. Update the process queue’s tail pointer if
	necessary. */
	/* Rimuove il primo elemento dalla coda dei processi puntata da head.
	Ritorna NULL se la coda è vuota. Altrimenti ritorna il puntatore
	all’elemento rimosso dalla lista. */
	
pcb_t *outProcQ(struct list_head *head, pcb_t *p);
	/* Remove the pcb pointed to by p from the process queue whose
	tail-pointer is pointed to by tp. Update the process queue’s tail
	pointer if necessary. If the desired entry is not in the indicated
	queue (an error condition), return NULL; otherwise, return p.
	Note that p can point to any element of the process queue. */
	/* Rimuove il PCB puntato da p dalla coda dei processi puntata da head.
	Se p non è presente nella coda, restituisce NULL. (NOTA: p può
	trovarsi in una posizione arbitraria della coda). */



//// TREES

int emptyChild(pcb_t *p);
	/* Return TRUE if the pcb pointed to by p has no children.
	Return FALSE otherwise. */
	/* Restituisce TRUE se il PCB puntato da p
	non ha figli, FALSE altrimenti */

void insertChild(pcb_t *prnt, pcb_t *p);
	/* Make the pcb pointed to by p a child of the pcb pointed to by
	prnt. */
	/* Inserisce il PCB puntato da p come figlio
	del PCB puntato da prnt.*/

pcb_t *removeChild(pcb_t *p);
	/* Make the first child of the pcb pointed to by p no longer a
	child of p. Return NULL if initially there were no children of p.
	Otherwise, return a pointer to this removed first child pcb. */
	/* Rimuove il primo figlio del PCB puntato
	da p. Se p non ha figli, restituisce NULL.*/
	
pcb_t *outChild(pcb_t *p);
	/* Make the pcb pointed to by p no longer the child of its parent.
	If the pcb pointed to by p has no parent, return NULL; otherwise,
	return p. Note that the element pointed to by p need not be the
	first child of its parent. */
	/* Rimuove il PCB puntato da p dalla lista dei figli del padre.
	Se il PCB puntato da p non ha un padre, restituisce NULL,
	altrimenti restituisce l’elemento rimosso (cioè p). A differenza della
	removeChild, p può trovarsi in una posizione arbitraria (ossia non è
	necessariamente il primo figlio del padre).*/