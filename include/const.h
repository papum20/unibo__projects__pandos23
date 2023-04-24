#ifndef PANDOS23_CONST_H
#define PANDOS23_CONST_H


/*	alias of static, for disambiguating its use to declare
	"private" functions, from its use as constant.
*/
#define HIDDEN static

/*
 * Semaphores values
 */
#define VAL_SEM_SYNC 0
#define VAL_SEM_MUTEX 1

/*
 * Types
 */
typedef unsigned int * uint_PTR;

#define TERMINATE_CURR_PROCESS 0
#define BIT_USER 1
#define BIT_KERNEL 0



#endif /* CONST_H */