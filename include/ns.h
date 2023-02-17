#ifndef NS_H
#define NS_H



#include "pcb.h"




/* NAMESPACES MANAGEMENT */

/**
 * Initialize all the lists of free namespaces with MAXPROC elements each.
 * This function should be called once during data structure initialization.
*/
extern void initNamespaces();
	
/**
 * Return the pointer to the namespace descriptor of type type
 * associated with the pcb p (or NULL).
*/
extern nsd_t *getNamespace(pcb_t *p, int type);

/**
 * Set the namespace ns as the namespace for p and all of its children.
 * Return FALSE in case of error, otherwise TRUE.
*/
extern int addNamespace(pcb_t *p, nsd_t *ns);

/**
 * Allocate and return a namespace of type type.
*/
extern nsd_t *allocNamespace(int type);

/**
 * Free the namespace ns.
*/
extern void freeNamespace(nsd_t *ns);



#endif