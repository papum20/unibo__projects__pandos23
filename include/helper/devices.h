
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


/* external devices
*/

/* device status codes */
#define DEV_NOT_INSTALLED		0
#define DEV_READY				1
#define DEV_ILLEGAL_OPCODE_ERR	2
#define DEV_BUSY				3
#define DEV_ERROR				4
#define TERM_SUCCESS			5	/* only terminals have an additional code */


#define DEV_STATUS_MASK	0x00000007
#define DEV_CMD_MASK		0x00000007

/* get the status from the recv/transm status field in terminal's register.
*/
#define DEV_STATUS(status) (status & DEV_STATUS_MASK)

/* set the status for the recv/transm status field in terminal's register.
*/
#define DEV_STATUS_SET(status, val) (status = (status & ~DEV_STATUS_MASK) | val)

/* set the command code for the recv/transm command field `cmd` in terminal's register.
*/
#define DEV_CMD_SET(cmd, val) (cmd = (cmd & ~DEV_CMD_MASK) | val)



#endif /* DEVICES_H */