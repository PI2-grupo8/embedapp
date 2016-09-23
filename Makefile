all: main.c rasp_uart.c
	gcc main.c -c
	gcc rasp_uart.c -c
	gcc *.o -o main