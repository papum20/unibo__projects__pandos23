#ifndef NS_H
#define NS_H



#include "pcb.h"




//// NAMESPACES MANAGEMENT

extern void initNamespaces();
	/* Initialize the type nsFree list to contain all the elements of the
	arrays static ns t type nsTable[MAXPROC]
	This method will be only called once during data structure initialization. */
	/* Inizializza tutte le liste dei namespace
	liberi. Questo metodo viene invocato
	una volta sola durante l’inizializzazione
	della struttura dati. */
	
extern nsd_t *getNamespace(pcb_t *p, int type);
	/* Return the pointer to the namespace descriptor of type type
	associated with the pcb p. */
	/* Ritorna il namespace di tipo type
	associato al processo p (o NULL).*/

extern int addNamespace(pcb_t *p, nsd_t *ns);
	/* Insert the namespace ns as the namespace for the correct type
	of p to ns. If the namespace is currently the base (i.e. there are
	no descriptor for the namespace), allocate a new descriptor from
	the type nsFree list, insert it in the correct list, initialize all of
	the fields, and proceed as above. If a new namespace descriptor
	needs to be allocated and the type nsFree list is empty, return
	TRUE. In all other cases return FALSE. */
	/* Associa al processo p e a tutti I suoi
	figli il namespace ns. Ritorna FALSE in
	caso di errore, TRUE altrimenti. */

extern nsd_t *allocNamespace(int type);
	/* Allocate a namespace from the type nsFree list and return
	to the user, this value can be used for the next calls to refer to
	this namespace. */
	/* Alloca un namespace di tipo type
	dalla lista corretta.*/

extern void freeNamespace(nsd_t *ns);
	/* Free a namespace, adding its list pointer (n link) to the correct type nsFree list. */
	/* Libera il namespace ns ri-inserendolo
	nella lista di namespace corretta.*/


#endif