#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "rasp_uart.h"
#include "sensors.h"
#include "actuators.h"

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

	struct timeval start, stop;

	FILE *fp_accel = fopen("accel.dat", "w");
	int i;

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

	while ((opt = getopt(argc, argv, "abcdefgh01234567")) != -1) {
		switch (opt) {
		case '0': // read_higro()
			printf("Leitura do higrometro: %i\n", read_higro());
			break;
		case '1': // read_air()
			printf("Umidade relativa do ar: %.2f %%\n", read_air());
			break;
		case '2': // read_temp()
			printf("Temperatura: %.2f C\n", read_temp());
			break;
		case '3': // read_sonar(0)
			printf("Ultrassom frontal: %.2f cm\n", read_sonar(0));
			break;
		case '4': // read_sonar(1)
			printf("Ultrassom traseiro: %.2f cm\n", read_sonar(1));
			break;
		case '5': // read_accel()
			printf("Leitura do acelerometro (x, y, z): %d, %d, %d\n", ad.ax, ad.ay, ad.az);
			break;
		case '6': // read_gyro()
			gd = read_gyro();
			printf("Leitura do giroscopio (x, y, z): %d, %d, %d\n", gd.gx, gd.gy, gd.gz);
			break;
		case '7': // read_gps()
			gpsd = read_gps();
			printf("Leitura do GPS (lat, lon): %.5f, %.5f\n", gpsd.lat, gpsd.lon);
			break;
		case 'h': // help
			print_help(argv[0]);
			break;
		case 'a': // L+
			direction('a');
			break;
		case 'b': // L
			direction('b');
			break;
		case 'c': // L-
			direction('c');
			break;
		case 'd': // DN
			direction('d');
			break;
		case 'e': // R-
			direction('e');
			break;
		case 'f': // R
			direction('f');
			break;
		case 'g': // R+
			direction('g');
			break;
		default:
			fprintf(stderr, "Usage: %s [-[01234567abcdefg]] [-h] <device>\n", argv[0]);
			exit(EXIT_FAILURE);
			break;
		}
	}

	uart_end();

	fclose(fp_accel);

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
	printf("  -a: direction L+\n");
	printf("  -b: direction L\n");
	printf("  -c: direction L-\n");
	printf("  -d: direction DN\n");
	printf("  -e: direction R-\n");
	printf("  -f: direction R\n");
	printf("  -g: direction R+\n");
	printf("  -h: print this message\n");
}
