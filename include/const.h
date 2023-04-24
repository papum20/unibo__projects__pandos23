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

typedef unsigned int * p_u_int;

#define bitUser 1
#define bitKernel 0



#endif /* CONST_H */