#ifndef PANDOS23_CONST_H
#define PANDOS23_CONST_H


/*
 * Basic types
 */
typedef unsigned int * uint_PTR;


/*	alias of static, for disambiguating its use to declare
	"private" functions, from its use as constant.
*/
#define HIDDEN static


/*
 * Status bits values
 */
#define BIT_KERNEL 0
#define BIT_USER 1


/*
 * Semaphores values
 */
#define VAL_SEM_SYNC 0
#define VAL_SEM_MUTEX 1


/*
 * Exceptions constants
 */
#define TERMINATE_CURR_PROCESS 0



#endif /* CONST_H */