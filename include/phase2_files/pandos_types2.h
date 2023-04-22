#ifndef PANDOS_TYPES_2_H_INCLUDED
#define PANDOS_TYPES_2_H_INCLUDED

/****************************************************************************
 *
 * This header file contains utility types definitions.
 *
 ****************************************************************************/

/* #include <umps3/umps/types.h>
*/



/* Pass Up Vector */
typedef struct passupvector {
	unsigned int tlb_refill_handler;
	unsigned int tlb_refill_stackPtr;
	unsigned int exception_handler;
	unsigned int exception_stackPtr;
} passupvector_t;

#define STATE_GPR_LEN 29

/* Processor state */
typedef struct state {
	unsigned int entry_hi;
	unsigned int cause;
	unsigned int status;
	unsigned int pc_epc;
	unsigned int gpr[STATE_GPR_LEN];
	unsigned int hi;
	unsigned int lo;
} state_t;

#define reg_at  gpr[0]
#define reg_v0  gpr[1]
#define reg_v1  gpr[2]
#define reg_a0  gpr[3]
#define reg_a1  gpr[4]
#define reg_a2  gpr[5]
#define reg_a3  gpr[6]
#define reg_t0  gpr[7]
#define reg_t1  gpr[8]
#define reg_t2  gpr[9]
#define reg_t3  gpr[10]
#define reg_t4  gpr[11]
#define reg_t5  gpr[12]
#define reg_t6  gpr[13]
#define reg_t7  gpr[14]
#define reg_s0  gpr[15]
#define reg_s1  gpr[16]
#define reg_s2  gpr[17]
#define reg_s3  gpr[18]
#define reg_s4  gpr[19]
#define reg_s5  gpr[20]
#define reg_s6  gpr[21]
#define reg_s7  gpr[22]
#define reg_t8  gpr[23]
#define reg_t9  gpr[24]
#define reg_gp  gpr[25]
#define reg_sp  gpr[26]
#define reg_fp  gpr[27]
#define reg_ra  gpr[28]
#define reg_HI  gpr[29]
#define reg_LO  gpr[30]


#endif /* PANDOS_TYPES_2_H_INCLUDED */