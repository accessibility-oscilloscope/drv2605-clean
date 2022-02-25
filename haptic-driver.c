#include "drv2605.h"
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

  init_i2c(&inst, ACTUATOR.i2c_bus, ACTUATOR.i2c_addr);
}
