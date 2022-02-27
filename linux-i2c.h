// linux specific i2c interface functions
#ifndef LINUX_I2C_H
#define LINUX_I2C_H

#include <stdint.h>
#include <stdio.h>

struct i2c_data {
  int fd; // file descriptor for linux /dev/ device
};

void init_i2c(struct i2c_data *i2c, unsigned int bus, unsigned int addr);

void write_reg(struct i2c_data *i2c, uint8_t reg, uint8_t val);

uint8_t read_reg(struct i2c_data *i2c, uint8_t reg);

void clear_bit_mask(struct i2c_data *i2c, uint8_t reg, uint8_t mask);

void set_bit_mask(struct i2c_data *i2c, uint8_t reg, uint8_t mask);

#endif
