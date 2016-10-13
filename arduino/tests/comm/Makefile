OBJ = comm_test.o rasp_uart.o sensors.o
CFLAGS = 
LDFLAGS = 
CC = gcc

# all: comm_test
# 	./comm_test -h /dev/ttyACM0

comm_test: ${OBJ}
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $<

.PHONY: clear
clear: ${OBJ} comm_test
	rm $^
