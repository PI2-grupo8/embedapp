#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "rasp_uart.h"
#include "actuators.h"
#include "sensors.h"

#define NEAR_DIST 10
#define FAR_DIST 20

#define USLEEP_CTE 500000

float state_machine(float front, float rear)
{
	const float near = NEAR_DIST;
	const float far = FAR_DIST;

	int ret_val = 0;

	if (front <= near) {
		if (rear <= near) {
			ret_val = -2;
		}
		else if (near <= rear && rear <= far) {
			ret_val = -2;
		}
		else if (far <= rear) {
			ret_val = -2;
		}
	}
	else if (near <= front && front <= far) {
		if (rear <= near) {
			ret_val = 2;
		}
		else if (near <= rear && rear <= far) {
			ret_val = 0;
		}
		else if (far <= rear) {
			ret_val = -2;
		}
	}
	else if (far <= front) {
		if (rear <= near) {
			ret_val = 2;
		}
		else if (near <= rear && rear <= far) {
			ret_val = 2;
		}
		else if (far <= rear) {
			ret_val = 2;
		}
	}

	return ret_val;
}

void wall_follower(float time)
{
	float front_dist = 0;
	float rear_dist = 0;

	float ti = get_time();
	float tf = 0;

	for (tf = ti; tf - ti <= time; tf = get_time()) {
		front_dist = read_sonar(0);
		rear_dist = read_sonar(1);

		printf("FRONT: %.4f -- BACK: %.4f\n", front_dist, rear_dist);

		direction(state_machine(front_dist, rear_dist));

		usleep(USLEEP_CTE);
	}
}

void measure()
{
	int higro;

	drill(1);
	sleep(5);
	higro = read_higro();
	drill(-1);
	sleep(5);

	printf("Leitura do higrometro: %d\n", higro);
}

int main (int argc, char *argv[])
{
	if (argc < 1)
		uart_init("/dev/ttyACM0");
	else
		uart_init(argv[1]);

	engine(1);
	wall_follower(30);
	engine(0);
	measure();
	engine(1);
	wall_follower(30);
	engine(0);

	uart_end();

	return 0;
}
