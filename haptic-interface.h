#ifndef HAPTIC_INTERFACE
#define HAPTIC_INTERFACE

#include <stdint.h>

#define FIFO_FILE "/tmp/fifo-haptic"

#define PLAYBACK_MODE_BIT 0

typedef struct haptic_struct {
  uint8_t control;
  uint8_t data;
} haptic;

#endif
