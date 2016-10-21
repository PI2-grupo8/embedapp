#include "rasp_uart.h"
#include <time.h>
#include <stdbool.h>

#define MOVE_FORWARD "\x00"
#define STOP "\x01"
#define PERFORM_MEASUREMENT "\x02"
#define SENSOR_SOIL_MOISTURE "\x03"
#define SENSOR_AIR_HUMIDITY "\x04"
#define SENSOR_AIR_TEMPERATURE "\x05"
#define SENSOR_GPS "\x06"
#define SWITCH "\x07"

void complete_line(int *lines_completed, int measurement_distance){
    // bool complete_line_drill = false;
    // int total_line_drills =
    // int current_total_drills = 0;

    // while(!complete_line_drill){
    //     while(total_moved < measurement_distance){
    //         total_moved = total_moved_in_line();
    //     }
    //     start_drill();
    // }

    (*lines_completed)++;
}

void make_first_curve(){

}

void make_second_curve(){

}

void make_big_curve(){

}

void checks_ended_circuit(int lines_completed, int total_number_lines){
    if(lines_completed < total_number_lines){
        // Circuit completed
        // Stop veicule
        printf("Circuit Ended!\n");
        exit(0);
    }
}

void start_navigation(int total_number_lines, int measurement_distance){
    int lines_completed = 0;

    while(lines_completed < total_number_lines){
        complete_line(&lines_completed, measurement_distance);

        checks_ended_circuit(lines_completed, total_number_lines);

        make_first_curve();
        make_second_curve();

        complete_line(&lines_completed, measurement_distance);

        checks_ended_circuit(lines_completed, total_number_lines);

        make_big_curve();
    }
}

double total_moved_in_line(){
    return 100.0;
}

int main (int argc, char* argv[]){
    int total_number_lines = 0;
    int measurement_distance = 0;
    FILE *settings_file = fopen("settings.conf", "r");

    fscanf(settings_file, "%d %d", &total_number_lines, &measurement_distance);

    // char* device = (argc < 2) ? MODEMDEVICE : argv[1];

    //uart_init(device);

    start_navigation(total_number_lines, measurement_distance);

    fclose(settings_file);

    // uart_end();

    return 0;
}