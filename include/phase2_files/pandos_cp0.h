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




#endif /* PANDOS_CP_0_H_INCLUDED */