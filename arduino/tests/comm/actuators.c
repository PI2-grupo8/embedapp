#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "rasp_uart.h"
#include "actuators.h"

void direction(char c)
{
	struct uart_data tx_buffer = {malloc(1), 1};

	tx_buffer.data[0] = c;
	uart_send(&tx_buffer);
}

