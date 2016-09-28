#include "rasp_uart.h"
#include <time.h>
#define MOVE_FORWARD "\x00"
#define STOP "\x01"
#define PERFORM_MEASUREMENT "\x02"
#define SENSOR_SOIL_MOISTURE "\x03"
#define SENSOR_AIR_HUMIDITY "\x04"
#define SENSOR_AIR_TEMPERATURE "\x05"
#define SENSOR_GPS "\x06"
#define SWITCH "\x07"

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

void turn_engine(char *engine, int speed){
    // send_message_arduino(engine, speed);
}

void stop_vehicle(){
    turn_engine("engine_a", 0);
    turn_engine("engine_b", 0);
}

void start_navigation(){
    // send_message_arduino(SWITCH);
    // int switch_button = receive_message_arduino();
    int switch_button = 0;
    // TODO: make function to read char and transform to int

    // clock_t start_time = clock(), diff;

    // diff = clock() - start_time;

    // int msec = diff * 1000 / CLOCKS_PER_SEC;
    // printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);

    if(switch_button){
        turn_engine("engine_a", 1);
        turn_engine("engine_b", 1);
    }
    else{
        printf("Equipament is turned off!\n");
        exit(0);
    }
}

void start_drill(){

}

double total_moved_in_line(){
    return 100.0;
}

void forward_navigation(){
    // while(!complete_line_drill){
    //     while(total_moved < drill_distance){
    //         total_moved = total_moved_in_line();
    //     }
    //     start_drill();
    // }
}

int main (int argc, char* argv[]){
    int number_lines = 0;
    int measurement_distance = 0;
    FILE *settings_file = fopen("settings.conf", "r");

    fscanf(settings_file, "%d %d", &number_lines, &measurement_distance);
    // TODO: configure Arduino with settings.

    // char* device = (argc < 2) ? MODEMDEVICE : argv[1];

    //uart_init(device);

    start_navigation();

    fclose(settings_file);

    // uart_end();

    return 0;
}