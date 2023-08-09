
/*
 * uMPS machine-specific constants, most notably bus & device memory
 * mapped register addresses.
 */

#ifndef PANDOS_ARCH_H_INCLUDED
#define PANDOS_ARCH_H_INCLUDED

/* #include <umps3/umps/arch.h>
*/



/*
 * Generalities
 */
#define WORD_SIZE 4
#define WS        WORD_SIZE

/* Device register size */
#define DEV_REG_SIZE_W   4
#define DEV_REG_SIZE     (DEV_REG_SIZE_W * WS)


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

#define IL_IPI              0	/* for multi-rocessing */
#define IL_CPUTIMER         1	/* Processor Local Timer (PLT), for scheduling */
#define IL_TIMER            2	/* Interval Timer, for pseudo-clock ticks */
#define IL_DISK             3
#define IL_FLASH            4
#define IL_ETHERNET         5
#define IL_PRINTER          6
#define IL_TERMINAL         7

#define EXT_IL_INDEX(il)    ((il) - DEV_IL_START)


/* Device register area */
#define DEV_REG_START           0x10000054
#define DEV_REG_ADDR(line, dev) (DEV_REG_START + ((line) - DEV_IL_START) * N_DEV_PER_IL * DEV_REG_SIZE + (dev) * DEV_REG_SIZE)

//Interrupting device bitmap start address
#define PENDING_BITMAP_START 0x1000003c

//Macro che ritorna una word contenente il device che richiede l'interrupt nella linea LINENO
#define INTR_CURRENT_BITMAP(LINE)	 (unsigned int *)(PENDING_BITMAP_START + (WORD_SIZE * (LINE - 3)))


#endif /* PANDOS_ARCH_H_INCLUDED */