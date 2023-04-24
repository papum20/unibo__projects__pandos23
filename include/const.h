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

//tempo di esecuzione cpu da quando è acceso il computer
#define getTOD_LO() (*((unsigned int*)TODLOADDR))

/* Macro to read the TOD clock */ //non so quale delle due è giusta TOL_LO o STCK
#define STCK(T)         ((T) = ((*((cpu_t *) TODLOADDR)) / (*((cpu_t *) TIMESCALEADDR))))


#endif /* CONST_H */