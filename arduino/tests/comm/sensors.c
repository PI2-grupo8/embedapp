#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "rasp_uart.h"
#include "sensors.h"

void read_higro()
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
	printf("Higrometer read: %i\n", read_var.i_var);
}

void read_air()
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

	printf("Air humidity read: %.2f\n", read_var.f_var);
}

void read_temp()
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

	printf("Temperature read: %.2f\n", read_var.f_var);
}

void read_sonar(int sonar_id)
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
		printf("Invalid sonar_id.\n");
		return;
	}
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i <= 3; i++)
		accum += (rx_buffer.data[i] << (i * 8));
	read_var.i_var = accum;

	printf("Front sonar reading: %.2f cm\n", read_var.f_var);
}

void read_accel()
{
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(6), 6};

	int16_t accel[3];
	int i;

	tx_buffer.data[0] = 5;
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i < 3; i++)
		accel[i] = rx_buffer.data[2*i] | (rx_buffer.data[2*i+1] << 8);
	printf("Accelerometer read:\n");
	printf("  Accel x: %d\n", accel[0]);
	printf("  Accel y: %d\n", accel[1]);
	printf("  Accel z: %d\n", accel[2]);
}

void read_gyro()
{
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(6), 6};

	int16_t gyro[3];
	int i;

	tx_buffer.data[0] = 6;
	uart_send(&tx_buffer);
	uart_recv(&rx_buffer);

	for (i = 0; i < 3; i++)
		gyro[i] = rx_buffer.data[2*i] | (rx_buffer.data[2*i+1] << 8);
	printf("Gyroscope read:\n");
	printf("  Gyro x: %d\n", gyro[0]);
	printf("  Gyro y: %d\n", gyro[1]);
	printf("  Gyro z: %d\n", gyro[2]);
}

void read_gps()
{
	union {
		float f_var;
		uint32_t i_var;
	} read_var;
	struct uart_data tx_buffer = {malloc(1), 1};
	struct uart_data rx_buffer = {malloc(8), 8};
	uint32_t accum = 0;
	float lat, lon;
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

	printf("GPS reading:\n");
	printf("  Latitude: %.4f\n", lat);
	printf("  Longitude: %.4f\n", lon);
}
