#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "rasp_uart.h"
#include "actuators.h"

void direction(char c)
{
	struct uart_data tx_buffer = {malloc(1), 1};
	switch(c)
	{
		case -3:
			tx_buffer.data[0] = 'c';
			break;
		case -2:
			tx_buffer.data[0] = 'b';
			break;
		case -1:
			tx_buffer.data[0] = 'a';
			break;
		case 0:
			tx_buffer.data[0] = 'd';
			break;
		case 1:
			tx_buffer.data[0] = 'e';
			break;
		case 2:
			tx_buffer.data[0] = 'f';
			break;
		case 3:
			tx_buffer.data[0] = 'g';
			break;
		default:
			return;
	}
	uart_send(&tx_buffer);
}

void engine(char c)
{
	struct uart_data tx_buffer = {malloc(1), 1};
	switch(c)
	{
		case -1:
			tx_buffer.data[0] = 'i';
			break;
		case 0:
			tx_buffer.data[0] = 'j';
			break;
		case 1:
			tx_buffer.data[0] = 'h';
			break;
		default:
			return;
	}
	uart_send(&tx_buffer);
	
}

