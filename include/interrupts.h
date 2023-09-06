#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "helper/const.h"
#include "exceptions.h"
#include "include/helper/interrupt_helper.h"

#include "include/helper/env_nucleus.h"



/* The interrupt handling function.
*/
extern void Interrupt_handler();

/* Complete an IO request, returning the result.
*/
extern void SYSCALL_DOIO_return(int *sem_key, unsigned int status);

/*  Controls whether there is a pending interrupt on a determinate line of a device
*/
extern int Check_pending_interrupt(int line, int device);

extern void Device_interrupt(int line);

extern void PLT_interrupt();

extern void IT_interrupt();

extern void Set_IO(int *sem_key, int status);

extern void Terminal_interrupt(int line);


#endif /* INTERRUPTS_H */