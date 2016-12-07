OBJ = actuators.o sensors.o tortuga_navigation.o rasp_uart.o

all: tortuga_navigation

tortuga_navigation: ${OBJ}
	gcc $^ -o $@

%.o: %.c
	gcc -c $< $a

clear:
	rm *.o main
