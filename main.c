#include "rasp_uart.h"
#define MOVE_FORWARD "\x00"
#define STOP "\x01"
#define PERFORM_MEASUREMENT "\x02"
#define SENSOR_SOIL_MOISTURE "\x03"
#define SENSOR_AIR_HUMIDITY "\x04"
#define SENSOR_AIR_TEMPERATURE "\x05"
#define SENSOR_GPS "\x06"

void send_message_arduino(char *sensor){
    struct uart_data buffer_send = {
        sensor, strlen(sensor)
    };

    buffer_send.data = sensor;
    buffer_send.length = 1;

    uart_send(&buffer_send);
    printf("Message send: %s\n", buffer_send.data);
}

void receive_message_arduino(){
    char texto[50] = "Teste";

    struct uart_data buffer_recv = {
        (char*) malloc (strlen(texto) * sizeof (char)), strlen(texto)
    };

    buffer_recv.length = 4;

    uart_recv(&buffer_recv);
    buffer_recv.data[buffer_recv.length] = 0;
    printf("Received data: %s\n", buffer_recv.data);
}

void stop_veicule(){

}

void start_navigation(){
    send_message_arduino(MOVE_FORWARD);
    receive_message_arduino();
}

int main (int argc, char* argv[]){
    int number_lines = 0;
    int measurement_distance = 0;
    FILE *settings_file = fopen("settings.conf", "r");

    fscanf(settings_file, "%d %d", &number_lines, &measurement_distance);

    // char* device = (argc < 2) ? MODEMDEVICE : argv[1];

    //uart_init(device);


    // start_navigation();

    fclose(settings_file);


    // uart_end();

    return 0;
}