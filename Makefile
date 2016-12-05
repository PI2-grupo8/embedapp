OBJ = actuators.o sensors.o main.o rasp_uart.o

all: main

main: ${OBJ}
	gcc $^ -o $@

%.o: %.c
	gcc -c $< $a

clear:
	rm *.o main
