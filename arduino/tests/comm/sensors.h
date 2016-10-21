#ifndef _SENSORS_H
#define _SENSORS_H

#include <stdint.h>

union {
	float f_val;
	uint32_t i_val;
} read_val;

struct gps_data {
	float lat, lon;
};

struct gyro_data {
	int16_t gx, gy, gz;
};

struct accel_data {
	int16_t ax, ay, az;
};

extern uint32_t read_higro();
extern float read_air();
extern float read_temp();
extern float read_sonar(int);
extern struct accel_data read_accel();
extern struct gyro_data read_gyro();
extern struct gps_data read_gps();

#endif
