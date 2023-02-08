#include "include/pcb.h"

/* namespace descriptor type */
typedef struct nsd_t {
	struct nsd_t	*n_next;	/* next element on the free list */
	int				n_type;		/* pointer to the semaphore */
} nsd_t;

nsd_t *getNamespace(pcb_t *p, int type);
	/* Return the pointer to the namespace descriptor of type type
	associated with the pcb p. */

nsd_t *allocNamespace(int type);
	/* Allocate a namespace from the type nsFree list and return
	to the user, this value can be used for the next calls to refer to
	this namespace. */

void freeNamespace(nsd_t *descriptor);
	/* Free a namespace, adding its list pointer (n link) to the correct type nsFree list. */

int addNamespace( *ns, pcb_t *p);
	/* Insert the namespace ns as the namespace for the correct type
	of p to ns. If the namespace is currently the base (i.e. there are
	no descriptor for the namespace), allocate a new descriptor from
	the type nsFree list, insert it in the correct list, initialize all of
	the fields, and proceed as above. If a new namespace descriptor
	needs to be allocated and the type nsFree list is empty, return
	TRUE. In all other cases return FALSE. */

void initNamespaces();
	/* Initialize the type nsFree list to contain all the elements of the
	arrays
	static ns t type nsTable[MAXPROC]
	This method will be only called once during data structure initialization. */