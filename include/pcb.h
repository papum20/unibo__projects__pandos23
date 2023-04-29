#ifndef PCB_H
#define PCB_H


#include "container_of.h"
#include "pandos_const.h"
#include "pandos_types.h"
#include "types.h"
#include "linux_list.h"
#include "pcb_help.h"


/**
 * Initialize the pcbFree_h list to contain MAXPROC elements.
 * This function should be called once during data structure initialization.
*/
extern void initPcbs();



/* ALLOCATION */

/**
 * Insert the element pointed to by p onto the pcbFree_h list.
*/
extern void freePcb(pcb_t *p);

/**
 * Remove, initialize (to 0/NULL) and return an unallocated pcb.
 * Return NULL if there is no available.
*/
extern pcb_t *allocPcb();



/* LISTS */

/**
 * Initialize the empty list head.
*/
extern void mkEmptyProcQ(struct list_head *head);

/**
 * Return TRUE if the list pointed to by head is empty,
 * FALSE otherwise.
*/
extern int emptyProcQ(struct list_head *head);

/**
 * Insert the element pointed to by p in the list pointed to by head.
 * Assumption: p must not be in any list.
*/
extern void insertProcQ(struct list_head *head, pcb_t *p);

/**
 * Return, without removing, the first element of head.
 * Return NULL if it is empty.
*/
extern pcb_t *headProcQ(struct list_head *head);

/**
 * Remove and return the first element of head.
 * Return NULL if it is empty.
*/
extern pcb_t *removeProcQ(struct list_head* head);
	

/**
 * Remove and return an element p, in any position, from the list head.
 * Return NULL if p is not in head.
*/
extern pcb_t *outProcQ(struct list_head *head, pcb_t *p);



/* TREES */

/**
 * Return TRUE if the pcb pointed to by p has no children.
 * Return FALSE otherwise.
*/
extern int emptyChild(pcb_t *p);

/**
 * Make the pcb pointed to by p a child of the pcb pointed to by prnt.
 * Assumption: p has no parent.
*/
extern void insertChild(pcb_t *prnt, pcb_t *p);

/**
 * Remove and return the first child of p.
 * Return NULL if p has no children.
*/
extern pcb_t *removeChild(pcb_t *p);

/**
 * Remove and return a child p, in any position, from its parent
 * (i.e. p may not be the first child)
 * Return NULL if p has no parent.
*/
extern pcb_t *outChild(pcb_t *p);


#endif /* PCB_H */