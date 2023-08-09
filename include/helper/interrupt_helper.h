#ifndef INTERRUPT_HELPER_H
#define INTERRUPT_HELPER_H

static inline int interrupt_line(unsigned int cause){
  cause >>= 8;

  // Apply a mask to keep only the least significant 8 bits
  cause &= 0x000000FF;

  // Find the position of the active bit
  int position = -1;
  while (cause) {
      cause >>= 1;
      position++;
  }

  return position;
}


#endif