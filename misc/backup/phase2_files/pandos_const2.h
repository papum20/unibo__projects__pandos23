/****************************************************************************
 *
 * This header file contains utility constants & macro definitions.
 *
 ****************************************************************************/


#ifndef PANDOS_CONST_2_H_INCLUDED
#define PANDOS_CONST_2_H_INCLUDED

/* #include <umps3/umps/const.h>
*/

/* timer, timescale, TOD-LO and other bus regs */
#define RAMBASEADDR     0x10000000
#define TODLOADDR       0x1000001C
#define INTERVALTMR     0x10000020
#define TIMESCALEADDR   0x10000024

/* Macro to load the Interval Timer */
#define LDIT(T)         ((*((cpu_t *) INTERVALTMR)) = (T) *(*((cpu_t *) TIMESCALEADDR)))

/* Macro to read the TOD clock */
#define STCK(T)         ((T) = ((*((cpu_t *) TODLOADDR)) / (*((cpu_t *) TIMESCALEADDR))))


#endif /* PANDOS_CONST_2_H_INCLUDED */