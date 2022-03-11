// linux specific implementation of read/write functions

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <syslog.h>
#include <unistd.h>

#include "linux-i2c.h"

void init_i2c(struct i2c_data *i2c, unsigned int bus, unsigned int addr) {
  char filename[20];
  snprintf(filename, 19, "/dev/i2c-%d", bus);
  i2c->fd = open(filename, O_RDWR);
  if (i2c->fd == -1) {
    syslog(LOG_ERR, "opening /dev/ file failed (%m)\n");
    exit(-1);
  }

  // setup device address
  if (ioctl(i2c->fd, I2C_SLAVE, addr) < 0) {
    syslog(LOG_ERR, "ioctl call failed (%m)\n");
    exit(-1);
  }
}

void write_reg(struct i2c_data *i2c, uint8_t reg, uint8_t val) {
  uint8_t buffer[2] = {reg, val};

  if (i2c->fd < 0) {
    syslog(LOG_ERR, "file descriptor does not exist\n");
    exit(-1);
  }
  if (write(i2c->fd, buffer, 2) != 2) {
    syslog(LOG_ERR, "write failed\n");
    exit(-1);
  }
}

uint8_t read_reg(struct i2c_data *i2c, uint8_t reg) {
  uint8_t buffer[1];

  if (write(i2c->fd, &reg, 1) != 1 || read(i2c->fd, buffer, 1) != 1) {
    syslog(LOG_ERR, "read failed\n");
    exit(-1);
  }

  return buffer[0];
}

void clear_bit_mask(struct i2c_data *i2c, uint8_t reg, uint8_t mask) {
  write_reg(i2c, reg, read_reg(i2c, reg) & ~mask);
}

void set_bit_mask(struct i2c_data *i2c, uint8_t reg, uint8_t mask) {
  write_reg(i2c, reg, read_reg(i2c, reg) | mask);
}
