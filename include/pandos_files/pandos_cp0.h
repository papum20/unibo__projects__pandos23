
/****************************************************************************
 *
 * This header file contains cpu definitions.
 *
 ****************************************************************************/


#ifndef PANDOS_CP_0_H_INCLUDED
#define PANDOS_CP_0_H_INCLUDED



/* #include <umps3/umps/cp0.h>
*/


/*
 * CP0 Status fields
 */
#define STATUS_IEc       0x00000001		/* interrupts enabled (current) */
#define STATUS_IEc_BIT   0
#define STATUS_KUc       0x00000002		/* kernel/user mode (current) */
#define STATUS_KUc_BIT   1

#define STATUS_IEp       0x00000004		/* interrupts enabled (previous) */
#define STATUS_IEp_BIT   2
#define STATUS_KUp       0x00000008		/* kernel/user mode (previous) */
#define STATUS_KUp_BIT   3

#define STATUS_IEo       0x00000010
#define STATUS_IEo_BIT   4
#define STATUS_KUo       0x00000020
#define STATUS_KUo_BIT   5

#define STATUS_IM_MASK       0x0000ff00
#define STATUS_IM(line)      (1U << (8 + (line)))
#define STATUS_IM_BIT(line)  (8 + (line))

#define STATUS_BEV       0x00400000
#define STATUS_BEV_BIT   22

#define STATUS_TE        0x08000000		/* PLT */
#define STATUS_TE_BIT    27

#define STATUS_CU0       0x10000000
#define STATUS_CU0_BIT   28
#define STATUS_CU1       0x20000000
#define STATUS_CU1_BIT   29
#define STATUS_CU2       0x40000000
#define STATUS_CU2_BIT   30
#define STATUS_CU3       0x80000000
#define STATUS_CU3_BIT   31

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


#endif /* PANDOS_CP_0_H_INCLUDED */