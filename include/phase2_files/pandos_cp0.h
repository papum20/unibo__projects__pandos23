#ifndef PANDOS_CP_0_H_INCLUDED
#define PANDOS_CP_0_H_INCLUDED


/****************************************************************************
 *
 * This header file contains cpu definitions.
 *
 ****************************************************************************/


/* #include <umps3/umps/cp0.h>
*/

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

#define TLB_EXCEPTION(x)(x>=EXC_MOD && x<=EXC_TLBS)
#define TRAP_EXCEPTION(x)((x>=EXC_ADEL && x<=EXC_DBE) || (x>=EXC_BP && x<=EXC_OV))

#endif /* PANDOS_CP_0_H_INCLUDED */