#include <fcntl.h>
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

#define ACTUATOR VYB_LRA

int main(int argc, char **argv) {
  struct ht_instance inst;
  struct haptic_struct interface;

  init_i2c(&inst.i2c, ACTUATOR.i2c_bus, ACTUATOR.i2c_addr);

  drv2605_program(&inst, &ACTUATOR);

  syslog(LOG_INFO, "programming and calibration complete\n");

  mkfifo(FIFO_FILE, 0666);
  int fd = open(FIFO_FILE, O_RDONLY);
  if (fd < 0) {
    syslog(LOG_ERR, "creating and opening fifo failed (%m)\n");
  }

  unsigned int rv = 0;

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
      // TODO: check validity of effect
      drv2605_set_waveform(&inst, 0, interface.data);
      drv2605_go(&inst);
      break;
    case MODE_REALTIME:
      drv2605_write_realtime(&inst, interface.data);
      break;
    default:
      syslog(LOG_ERR, "how did you get here? bad mode");
    }
  }
}
