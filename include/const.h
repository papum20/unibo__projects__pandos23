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

typedef unsigned int * uint_PTR;

//tempo di esecuzione cpu da quando è acceso il computer
#define getTOD_LO() (*((unsigned int*)TODLOADDR))

/* Macro to read the TOD clock */ //non so quale delle due è giusta TOL_LO o STCK
#define STCK(T)         ((T) = ((*((cpu_t *) TODLOADDR)) / (*((cpu_t *) TIMESCALEADDR))))

/*
 * CP0 Cause fields
 */
#define CAUSE_EXCCODE_MASK     0x0000007c
#define CAUSE_EXCCODE_BIT      2
#define CAUSE_GET_EXCCODE(x)   (((x) & CAUSE_EXCCODE_MASK) >> CAUSE_EXCCODE_BIT)

/* Exception codes - naming follows standard MIPS mnemonics */
#define EXC_INT                0
#define EXC_MOD                1
#define EXC_TLBL               2
#define EXC_TLBS               3
#define EXC_ADEL               4
#define EXC_ADES               5
#define EXC_IBE                6
#define EXC_DBE                7
#define EXC_SYS                8
#define EXC_BP                 9
#define EXC_RI                 10
#define EXC_CPU                11
#define EXC_OV                 12

#define TLB_EXCEPTION(x) (x>=EXC_MOD && x<=EXC_TLBS)  
#define TRAP_EXCEPTION(x)(x>=EXC_ADEL && x<=EXC_OV) 


#define bitUser 1
#define bitKernel 0

#endif /* CONST_H */