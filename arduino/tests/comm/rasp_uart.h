#ifndef _RASP_UART_H
#define _RASP_UART_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#ifndef BAUDRATE
#	define BAUDRATE B9600
#endif

#ifndef MODEMDEVICE
#	define MODEMDEVICE "/dev/ttyACM0"
#endif

struct uart_data {
	uint8_t* data;
	size_t length;
};

extern int uart0_filestream;
extern struct termios oldtio, newtio;

extern int uart_init (const char*);
extern int uart_send (struct uart_data*);
extern int uart_recv (struct uart_data*);
extern void uart_end ();

#endif
