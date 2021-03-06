#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "drv2605.h"
#include "haptic-interface.h"
#include "linux-i2c.h"

const struct ht_hardware_config VYB_COIN_ERM = {
    .i2c_bus = 1,
    .i2c_addr = 0x5A,
    .type = ACTUATOR_ERM,
    .rated_mv = 3000,
    .clamp_mv = 3200,
    // .resonance_hz = , // not used for ERM
    .library = LIB_TS2200_C,
};

const struct ht_hardware_config VYB_LRA = {
    .i2c_bus = 1,
    .i2c_addr = 0x5A,
    .type = ACTUATOR_LRA,
    .rated_mv = 1800,
    .clamp_mv = 1800,
    .resonance_hz = 235,
    .library = LIB_LRA,
};

#define ACTUATOR VYB_COIN_ERM

int main(int argc, char **argv) {
  struct ht_instance inst;
  struct haptic_struct interface;

  openlog("haptic-driver", 0, LOG_USER);

  if (argc < 2) {
    syslog(LOG_ERR, "usage: %s $INPUT_FIFO\n", argv[0]);
    exit(1);
  }

  // closed vs open loop, we only do open-loop ERM with
  // this version of the driver
  int is_closed_loop = ACTUATOR.type == ACTUATOR_LRA ? 1 : 0;

  init_i2c(&inst.i2c, ACTUATOR.i2c_bus, ACTUATOR.i2c_addr);
  drv2605_program(&inst, &ACTUATOR);
  syslog(LOG_INFO, "programming and calibration complete\n");

  int fd = open(argv[1], O_RDONLY);
  if (fd == -1) {
    syslog(LOG_ERR, "opening fifo failed (%m)\n");
    exit(-1);
  }

  unsigned int rv = 0;
  uint8_t val_to_write = 0;

  syslog(LOG_INFO, "haptic-driver: initialization complete");

  while (1) {

    rv = read(fd, &interface, sizeof(interface));

    if (rv == 0) {
      continue;
    } else if (rv != sizeof(interface)) {
      syslog(LOG_INFO, "read returned bad size");
      continue;
    }

    if (interface.control && inst.mode != MODE_INT_TRIG) {
      drv2605_set_mode(&inst, MODE_INT_TRIG);
    } else if (!interface.control && inst.mode != MODE_REALTIME) {
      drv2605_set_mode(&inst, MODE_REALTIME);
    }

    switch (inst.mode) {
    case MODE_INT_TRIG:
      if (interface.data >= 1 && interface.data <= 123) {
        drv2605_set_waveform(&inst, 0, interface.data);
        drv2605_go(&inst);
      } else {
        syslog(LOG_INFO, "invalid effect");
      }
      break;
    case MODE_REALTIME:
      val_to_write = is_closed_loop ? interface.data : interface.data / 2;
      drv2605_write_realtime(&inst, val_to_write);
      break;
    default:
      syslog(LOG_ERR, "how did you get here? bad mode");
    }
  }
}
