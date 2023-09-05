#ifndef INTERRUPT_HELPER_H
#define INTERRUPT_HELPER_H

#include "include/pandos_files/pandos_cp0.h"
#include "include/helper/state.h"



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