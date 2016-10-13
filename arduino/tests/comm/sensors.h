#ifndef _SENSORS_H
#define _SENSORS_H

#include <stdint.h>

union {
	float f_val;
	uint32_t i_val;
} read_val;

extern void read_higro();
extern void read_air();
extern void read_temp();
extern void read_sonar(int);
extern void read_accel();
extern void read_gyro();
extern void read_gps();

#endif
