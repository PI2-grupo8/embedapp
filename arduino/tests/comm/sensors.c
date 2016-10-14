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

	printf("Air humidity read: %f\n", read_var.f_var);
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

	printf("Temperature read: %f\n", read_var.f_var);
}

void read_sonar(int sonar_id)
{
	printf("Sonar read\n");
}

void read_accel()
{
	printf("Accelerometer read\n");
}

void read_gyro()
{
	printf("Gyroscope read\n");
}

void read_gps()
{
	printf("GPS read\n");
}
