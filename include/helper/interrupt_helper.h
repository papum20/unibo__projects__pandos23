#ifndef INTERRUPT_HELPER_H
#define INTERRUPT_HELPER_H

#include "pandos_files/pandos_cp0.h"
#include "helper/state.h"



/*
 * CONSTANTS
 */

#define RETURN_DOIO_SUCCESS	0
#define RETURN_DOIO_FAILURE	-1


#define TERM_WR_LINE 7
#define TERM_RD_LINE 8
#define MULTIPLE_LINE_DEVICES 6
#define ACK_DEVICE 1
#define PLT_RESET 4294967293
#define IT_RESET 100000
#define WRITE_TERMINAL 0
#define READ_TERMINAL 1



/*
 * FUNCTIONS
 */

static inline int interrupt_line(unsigned int cause) {

  // Apply a mask to keep only the least significant 8 bits
  cause &= CAUSE_IP_MASK;

  cause >>= CAUSE_IP_START;

  // Find the position of the active bit
  int position = -1;
  while (cause) {
      cause >>= 1;
      position++;
  }

  return position;
}


#endif