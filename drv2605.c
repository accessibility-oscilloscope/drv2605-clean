#include "drv2605.h"
#include "linux-i2c.h"

// knowledge blessed by the TI kernel driver gods
// "close enough"?
static unsigned int drv2605_calculate_voltage(unsigned int mv) {
  return (mv * 255 / 5600);
}

void drv2605_program(struct ht_instance *inst,
                     struct ht_hardware_config *conf) {
  // enter calibration mode
  write_reg(&inst->i2c, REG_MODE, MODE_AUTOCAL);

  // hardware specific settings
  switch (conf->type) {
  case ACTUATOR_ERM:
    clear_bit_mask(&inst->i2c, REG_FEEDBACK, 1 << REG_FEEDBACK_ERM_LRA_SHIFT);
    break;
  case ACTUATOR_LRA:
    set_bit_mask(&inst->i2c, REG_FEEDBACK, 1 << REG_FEEDBACK_ERM_LRA_SHIFT);
    clear_bit_mask(&inst->i2c, REG_CONTROL1, REG_CONTROL1_DRIVE_TIME_MASK);
    // see Table 24
    unsigned int drive_time = 5000 / conf->resonance_hz - 5;
    set_bit_mask(&inst->i2c, REG_CONTROL1, drive_time);
    break;
  }

  write_reg(&inst->i2c, REG_CONTROL4,
            CAL_1000_1200_MS << REG_CONTROL4_AUTO_CAL_TIME_SHIFT);
  write_reg(&inst->i2c, REG_RATEDV, drv2605_calculate_voltage(conf->rated_mv));
  write_reg(&inst->i2c, REG_CLAMPV, drv2605_calculate_voltage(conf->clamp_mv));

  write_reg(&inst->i2c, REG_GO, 1);
  write_reg(&inst->i2c, REG_LIBRARY, conf->library);

  drv2605_set_mode(inst, MODE_INT_TRIG);
}

void drv2605_set_mode(struct ht_instance *inst, mode_E mode) {
  write_reg(&inst->i2c, REG_MODE, mode);
  inst->mode = mode;
}

void drv2605_write_realtime(struct ht_instance *inst, uint8_t val) {
  write_reg(&inst->i2c, REG_RTPIN, val);
}

void drv2605_set_waveform(struct ht_instance *inst, uint8_t slot,
                          uint8_t effect) {
  write_reg(&inst->i2c, REG_WAVESEQ1 + slot, effect);
}

void drv2605_go(struct ht_instance *inst) { write_reg(&inst->i2c, REG_GO, 1); }
