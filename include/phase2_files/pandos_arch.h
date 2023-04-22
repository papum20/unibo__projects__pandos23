#ifndef PANDOS_ARCH_H_INCLUDED
#define PANDOS_ARCH_H_INCLUDED

/*
 * uMPS machine-specific constants, most notably bus & device memory
 * mapped register addresses.
 */

/* #include <umps3/umps/arch.h>
*/



/*
 * Interrupt lines
 */

#define N_INTERRUPT_LINES   8

#define N_IL                N_INTERRUPT_LINES

/* Number of interrupt lines available to devices */
#define N_EXT_IL            5

/* Devices per interrupt line */
#define N_DEV_PER_IL        8

#define DEV_IL_START        (N_INTERRUPT_LINES - N_EXT_IL)

#define IL_IPI              0
#define IL_CPUTIMER         1
#define IL_TIMER            2
#define IL_DISK             3
#define IL_FLASH            4
#define IL_ETHERNET         5
#define IL_PRINTER          6
#define IL_TERMINAL         7

#define EXT_IL_INDEX(il)    ((il) - DEV_IL_START)


#endif /* PANDOS_ARCH_H_INCLUDED */