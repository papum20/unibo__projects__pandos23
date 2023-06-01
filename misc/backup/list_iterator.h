

/****************************************************************************
 *
 * generic struct to implement a list of elements indicated by their pointer.
 *
 ****************************************************************************/


#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H


#include "linux_list.h"

#include "types.h"


typedef struct list_iterator {
	struct list_head list;
	void *p;
} list_iterator;



/* declare and init an iterator with name */
#define LIST_ITERATOR(name) \
	list_iterator name = {LIST_HEAD_INIT(name), NULL}


/* return the iterator element, casted to type */
#define iterator_get(it, type) ((type)iterator->p)


/*
 * list operations.
 */
#define iterator_add_tail(new, it) list_add_tail(new, it->list)



#endif /* LIST_ITERATOR_H */