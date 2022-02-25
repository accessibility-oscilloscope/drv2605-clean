// linux specific implementation of read/write functions

#include <stdint.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>

#include "linux_i2c.h"

void write_reg(struct ht_instance *inst, uint8_t reg, uint8_t val) {
  uint8_t buffer[2] = {reg, val};

  if (inst->fd < 0) {
    syslog(LOG_ERR, "file descriptor does not exist");
    abort();
  }
  if (write(inst->fd, buffer, 2) != 2) {
    syslog(LOG_ERR, "write failed");
    abort();
  }
}

uint8_t read_reg(struct ht_instance *inst, uint8_t reg) {
  uint8_t buffer[1];

  if (write(inst->fd, &reg, 1) != 1 || read(inst->fd, buffer, 1) != 1) {
    syslog(LOG_ERR, "read failed");
    abort();
  }

  return buffer[0];
}
