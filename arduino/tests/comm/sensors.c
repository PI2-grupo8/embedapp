#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "rasp_uart.h"
#include "sensors.h"

uint32_t read_higro()
{
	union {
		float f_var;
		uint32_t i_var;
	} read_var;
	struct uart_data rx_buffer = {malloc(2), 2};
	struct uart_data tx_buffer = {malloc(1), 1};

	tx_buffer.data[0] = 0;

	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);
	read_var.i_var = rx_buffer.data[0] + (rx_buffer.data[1] << 8);

	return read_var.i_var;
}

float read_air()
{
	union {
		float f_var;
		uint32_t i_var;
	} read_var;
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(4), 4};
	uint32_t accum = 0;
	int i;

	tx_buffer.data[0] = 1;
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i <= 3; i++)
		accum += (rx_buffer.data[i] << (i * 8));
	read_var.i_var = accum;

	return read_var.f_var;
}

float read_temp()
{
	union {
		float f_var;
		uint32_t i_var;
	} read_var;
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(4), 4};
	uint32_t accum = 0;
	int i;

	tx_buffer.data[0] = 2;
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i <= 3; i++)
		accum += (rx_buffer.data[i] << (i * 8));
	read_var.i_var = accum;

	return read_var.f_var;
}

float read_sonar(int sonar_id)
{
	union {
		float f_var;
		uint32_t i_var;
	} read_var;
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(4), 4};
	uint32_t accum = 0;
	int i;

	switch (sonar_id)
	{
	case 0:
		tx_buffer.data[0] = 3;
		break;
	case 1:
		tx_buffer.data[0] = 4;
		break;
	default:
		return -1;
	}
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i <= 3; i++)
		accum += (rx_buffer.data[i] << (i * 8));
	read_var.i_var = accum;

	return read_var.f_var;
}

struct accel_data read_accel()
{
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(6), 6};

	int16_t accel[3];
	struct accel_data ad;
	int i;

	tx_buffer.data[0] = 5;
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i < 3; i++)
		accel[i] = rx_buffer.data[2*i] | (rx_buffer.data[2*i+1] << 8);
	ad.ax = accel[0];
	ad.ay = accel[1];
	ad.az = accel[2];

	return ad;
}

struct gyro_data read_gyro()
{
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(6), 6};

	int16_t gyro[3];
	struct gyro_data gd;
	int i;

	tx_buffer.data[0] = 6;
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i < 3; i++)
		gyro[i] = rx_buffer.data[2*i] | (rx_buffer.data[2*i+1] << 8);
	gd.gx = gyro[0];
	gd.gx = gyro[1];
	gd.gx = gyro[2];

	return gd;
}

struct gps_data read_gps()
{
	union {
		float f_var;
		uint32_t i_var;
	} read_var;
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(8), 8};
	uint32_t accum = 0;
	float lat, lon;
	struct gps_data gd;
	int i;

	tx_buffer.data[0] = 7;

	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i <= 3; i++)
		accum += (rx_buffer.data[i] << (i * 8));
	read_var.i_var = accum;
	lat = read_var.f_var;

	for (i = 0; i <= 3; i++)
		accum += (rx_buffer.data[i+4] << (i * 8));
	read_var.i_var = accum;
	lon = read_var.f_var;

	gd.lat = lat;
	gd.lon = lon;

	return gd;
}
