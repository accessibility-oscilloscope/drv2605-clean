#ifndef DRV2605_H
#define DRV2605_H

#include "linux-i2c.h"

typedef enum {
  REG_STATUS = 0x00,
  REG_MODE = 0x01,
  REG_RTPIN = 0x02,
  REG_LIBRARY = 0x03,
  REG_WAVESEQ1 = 0x04,
  REG_WAVESEQ2 = 0x05,
  REG_WAVESEQ3 = 0x06,
  REG_WAVESEQ4 = 0x07,
  REG_WAVESEQ5 = 0x08,
  REG_WAVESEQ6 = 0x09,
  REG_WAVESEQ7 = 0x0A,
  REG_WAVESEQ8 = 0x0B,
  REG_GO = 0x0C,
  REG_OVERDRIVE = 0x0D,
  REG_SUSTAINPOS = 0x0E,
  REG_SUSTAINREG = 0x0F,
  REG_BREAK = 0x10,
  REG_AUDIOCTRL = 0x11,
  REG_AUDIOLVL = 0x12,
  REG_AUDIOMAX = 0x13,
  REG_AUDIOOUTMIX = 0x14,
  REG_AUDIOOUTMAX = 0x15,
  REG_RATEDV = 0x16,
  REG_CLAMPV = 0x17,
  REG_AUTOCALCOMP = 0x18,
  REG_FEEDBACK = 0x1A,
  REG_CONTROL1 = 0x1B,
  REG_CONTROL2 = 0x1C,
  REG_CONTROL3 = 0x1D,
  REG_CONTROL4 = 0x1E,
  REG_VBAT = 0x21,
  REG_LRARESON = 0x22,
} reg_E;

typedef enum {
  ACTUATOR_ERM = 0, // open loop
  ACTUATOR_LRA = 1, // closed loop
} actuator_type_E;

// Table 7 (values), Table 1 (explanation)
typedef enum {
  LIB_TS2200_A = 1,
  LIB_TS2200_B = 2,
  LIB_TS2200_C = 3,
  LIB_TS2200_D = 4,
  LIB_TS2200_E = 5,
  LIB_LRA = 6,
} library_E;

typedef enum {
  MODE_INT_TRIG = 0x00,
  MODE_EXTTRIGEDGE = 0x01,
  MODE_EXTTRIGLVL = 0x02,
  MODE_PWMANALOG = 0x03,
  MODE_AUDIOVIBE = 0x04,
  MODE_REALTIME = 0x05,
  MODE_DIAGNOSIS = 0x06,
  MODE_AUTOCAL = 0x07,
} mode_E;

typedef enum {
  CAL_150_350_MS = 0,
  CAL_250_450_MS = 1,
  CAL_500_700_MS = 2,
  CAL_1000_1200_MS = 3,
} auto_cal_time_E;

#define REG_FEEDBACK_ERM_LRA_SHIFT 7
#define REG_CONTROL1_DRIVE_TIME_MASK 0x1F
#define REG_CONTROL4_AUTO_CAL_TIME_SHIFT 4

struct ht_hardware_config {
  unsigned int i2c_bus;  // linux i2c bus number
  unsigned int i2c_addr; // linux i2c bus number
  actuator_type_E type;
  unsigned int rated_mv;     // rated actuator voltage in mv
  unsigned int clamp_mv;     // overdrive clamp voltage in mv
  unsigned int resonance_hz; // hardware resonance frequency (LRA only)
  library_E library;         // effect library to use
};

struct ht_instance {
  mode_E mode;
  struct i2c_data i2c;
};

void drv2605_program(struct ht_instance *inst, struct ht_hardware_config *conf);

void drv2605_set_mode(struct ht_instance *inst, mode_E mode);

void drv2605_write_realtime(struct ht_instance *inst, uint8_t val);

void drv2605_set_waveform(struct ht_instance *inst, uint8_t slot,
                          uint8_t effect);

void drv2605_go(struct ht_instance *inst);

#endif
