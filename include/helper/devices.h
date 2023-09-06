
/****************************************************************************
 *
 * This header file contains definitions about devices and their registers, codes, arguments, values.
 *
 ****************************************************************************/


#ifndef DEVICES_H
#define DEVICES_H


#include "pandos_files/pandos_const.h"


/* TOD clock
*/

/* current TOD clock value */
#define TODCK ( (*((cpu_t *) TODLOADDR)) / (*((cpu_t *) TIMESCALEADDR)) )


/* terminals
*/

/* terminal status codes  */
#define TERM_READY 1


#endif /* DEVICES_H */