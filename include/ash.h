#include "pcb.h"




//// ASH MANAGEMENT

static int insertBlocked(int *semAdd, pcb_t *p);
	/* Insert the pcb pointed to by p at the tail of the process queue
	associated with the semaphore whose physical address is semAdd
	and set the semaphore address of p to semAdd. If the semaphore
	is currently not active (i.e. there is no descriptor for it in the
	ASH), allocate a new descriptor from the semdFree list, insert it
	in the ASH, initialize all of the fields (i.e. set s semAdd to semAdd,
	and s procq to mkEmptyProcQ()), and proceed as above. If a new
	semaphore descriptor needs to be allocated and the semdFree list
	is empty, return TRUE. In all other cases return FALSE. */
	/* Viene inserito il PCB puntato da p nella coda dei
	processi bloccati associata al SEMD con chiave
	semAdd. Se il semaforo corrispondente non è
	presente nella ASH, alloca un nuovo SEMD dalla
	lista di quelli liberi (semdFree) e lo inserisce nella
	ASH, settando I campi in maniera opportuna (i.e.
	key e s_procQ). Se non è possibile allocare un
	nuovo SEMD perché la lista di quelli liberi è vuota,
	restituisce TRUE. In tutti gli altri casi, restituisce
	FALSE. */

static pcb_t *removeBlocked(int *semAdd);
	/* Search the ASH for a descriptor of this semaphore. If none is
	found, return NULL; otherwise, remove the first (i.e. head) pcb
	from the process queue of the found semaphore descriptor and
	return a pointer to it. If the process queue for this semaphore
	becomes empty (emptyProcQ(s procq) is TRUE), remove the
	semaphore descriptor from the ASH and return it to the semdFree
	list. */
	/* Ritorna il primo PCB dalla coda dei processi
	bloccati (s_procq) associata al SEMD della
	ASH con chiave semAdd. Se tale descrittore
	non esiste nella ASH, restituisce NULL.
	Altrimenti, restituisce l’elemento rimosso. Se
	la coda dei processi bloccati per il semaforo
	diventa vuota, rimuove il descrittore
	corrispondente dalla ASH e lo inserisce nella
	coda dei descrittori liberi (semdFree_h). */

static pcb_t *outBlocked(pcb_t *p);
	/* Remove the pcb pointed to by p from the process queue associated with p’s semaphore (p→ p semAdd) on the ASH. If pcb
	pointed to by p does not appear in the process queue associated
	with p’s semaphore, which is an error condition, return NULL;
	otherwise, return p. */
	/* Rimuove il PCB puntato da p dalla coda del semaforo
	su cui è bloccato (indicato da p->p_semAdd). Se il PCB
	non compare in tale coda, allora restituisce NULL
	(condizione di errore). Altrimenti, restituisce p. Se la
	coda dei processi bloccati per il semaforo diventa
	vuota, rimuove il descrittore corrispondente dalla ASH
	e lo inserisce nella coda dei descrittori liberi */

static pcb_t *headBlocked(int *semAdd);
	/* Return a pointer to the pcb that is at the head of the process
	queue associated with the semaphore semAdd. Return NULL if
	semAdd is not found on the ASH or if the process queue associated
	with semAdd is empty. */
	/* Restituisce (senza rimuovere) il puntatore al PCB che si
	trova in testa alla coda dei processi associata al SEMD
	con chiave semAdd. Ritorna NULL se il SEMD non
	compare nella ASH oppure se compare ma la sua coda
	dei processi è vuota.*/

initASH();
	/* Initialize the semdFree list to contain all the elements of the array
	static semd t semdTable[MAXPROC]
	This method will be only called once during data structure initialization. */
	/* Inizializza la lista dei semdFree in
	modo da contenere tutti gli elementi
	della semdTable. Questo metodo
	viene invocato una volta sola durante
	l’inizializzazione della struttura dati. */