#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "helper/const.h"
#include "exceptions.h"

int dev_semaphores[MULTIPLE_LINE_DEVICES][N_DEV_PER_IL];

/* The interrupt initializing function.
*/
extern void Interrupt_init();


/* The interrupt handling function.
*/
extern void Interrupt_handler();

/* Complete an IO request, returning the result.
*/
extern void SYSCALL_DOIO_return();

/*  Controls whether there is a pending interrupt on a determinate line of a device
*/
extern void Check_pending_interrupt(int line, int device);

extern void Device_interrupt(int line);

extern void Set_IO(int *sem_key, int status);



#endif /* INTERRUPTS_H */