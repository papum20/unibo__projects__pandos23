
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

/* terminal status codes */
#define TERM_NOT_INSTALLED		0
#define TERM_READY				1
#define TERM_ILLEGAL_OPCODE_ERR	2
#define TERM_BUSY				3
#define TERM_ERROR				4
#define TERM_SUCCESS			5


#define TERM_STATUS_MASK	0x00000007
#define TERM_CMD_MASK		0x00000007

/* get the status from the recv/transm status field in terminal's register.
*/
#define TERM_STATUS(status) (status & TERM_STATUS_MASK)

/* set the status for the recv/transm status field in terminal's register.
*/
#define TERM_STATUS_SET(status, val) (status = (status & ~TERM_STATUS_MASK) | val)

/* set the command code for the recv/transm command field `cmd` in terminal's register.
*/
#define TERM_CMD_SET(cmd, val) (cmd = (cmd & ~TERM_CMD_MASK) | val)



#endif /* DEVICES_H */