#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rasp_uart.h"
#include "sensors.h"

void print_help (char*);

int main(int argc, char **argv)
{
	int opt;
	int status;
	char device[512];
	char *last_arg = argv[argc-1];

	struct accel_data ad;
	struct gyro_data gd;
	struct gps_data gpsd;

	if (last_arg[0] != '-' && argc > 1)
		strncpy(device, last_arg, 512);
	else
		scanf("%[^\n]%*c", device);

	//printf("Trying to initiate device at %s.\n", device);
	if ((status = uart_init(device)) != 0) {
		fprintf(stderr, "Error opening device: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	//printf("Device initiated succesfully.\n");

	while ((opt = getopt(argc, argv, "h01234567")) != -1) {
		switch (opt) {
		case '0': // read_higro()
			read_higro();
			break;
		case '1': // read_air()
			read_air();
			break;
		case '2': // read_temp()
			read_temp();
			break;
		case '3': // read_sonar(0)
			printf("\nSonar: %.2f cm\n", read_sonar(0));
			break;
		case '4': // read_sonar(1)
			read_sonar(1);
			break;
		case '5': // read_accel()
			ad = read_accel();
			printf("%d, %d, %d\n", ad.ax, ad.ay, ad.az);
			break;
		case '6': // read_gyro()
			gd = read_gyro();
			printf("%d, %d, %d\n", gd.gx, gd.gy, gd.gz);
			break;
		case '7': // read_gps()
			read_gps();
			break;
		case 'h': // help
			print_help(argv[0]);
			break;
		default:
			fprintf(stderr, "Usage: %s [-[01234567]] [-h] <device>\n", argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}

	uart_end();

	exit(EXIT_SUCCESS);
}

void print_help (char *name)
{
	printf("Usage: %s [-[01234567]] [-h] <device>\n", name);
	printf("  -0: read higrometer\n");
	printf("  -1: read relative air humidity\n");
	printf("  -2: read read temperature\n");
	printf("  -3: read front sonar\n");
	printf("  -4: read rear sonar\n");
	printf("  -5: read accelerometer\n");
	printf("  -6: read gyroscope\n");
	printf("  -7: read GPS\n");
	printf("  -h: print this message\n");
}
