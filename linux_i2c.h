// linux specific i2c interface functions
#ifndef LINUX_I2C_H
#define LINUX_I2C_H

#include <stdint.h>

struct ht_instance {
  int fd; // file descriptor for linux /dev/ device
};

void write_reg(struct ht_instance *inst, uint8_t reg, uint8_t val);

uint8_t read_reg(struct ht_instance *inst, uint8_t reg);

#endif
