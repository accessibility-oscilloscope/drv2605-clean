// linux specific implementation of read/write functions

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdlib.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "linux_i2c.h"

void init_i2c(struct ht_instance *inst, unsigned int bus, unsigned int addr) {
  char filename[20];
  snprintf(filename, 19, "/dev/i2c-%d", bus);
  inst->fd = open(filename, O_RDWR);
  if (inst->fd < 0) {
    syslog(LOG_ERR, "opening /dev/ file failed (%m)\n");
  }

  // setup device address
  if (ioctl(inst->fd, I2C_SLAVE, addr) < 0) {
    syslog(LOG_ERR, "ioctl call failed (%m)\n");
  }
}

void write_reg(struct ht_instance *inst, uint8_t reg, uint8_t val) {
  uint8_t buffer[2] = {reg, val};

  if (inst->fd < 0) {
    syslog(LOG_ERR, "file descriptor does not exist\n");
    abort();
  }
  if (write(inst->fd, buffer, 2) != 2) {
    syslog(LOG_ERR, "write failed\n");
    abort();
  }
}

uint8_t read_reg(struct ht_instance *inst, uint8_t reg) {
  uint8_t buffer[1];

  if (write(inst->fd, &reg, 1) != 1 || read(inst->fd, buffer, 1) != 1) {
    syslog(LOG_ERR, "read failed\n");
    abort();
  }

  return buffer[0];
}

void clear_bit_mask(struct ht_instance *inst, uint8_t reg, uint8_t mask) {
  write_reg(inst, reg, read_reg(inst, reg) & ~mask);
}

void set_bit_mask(struct ht_instance *inst, uint8_t reg, uint8_t mask) {
  write_reg(inst, reg, read_reg(inst, reg) | mask);
}
