#include "arduino/tests/comm/rasp_uart.h"
#include "arduino/tests/comm/sensors.c"
#include <stdbool.h>
#include "unistd.h"

#define CLOSE_DISTANCE 10
#define FAR_DISTANCE 20

void start_drill(){
    printf("Ready to start drill!\n");
}

void wall_follower(){
    float top_sonar_value = 0;
    float bottom_sonar_value = 0;

    top_sonar_value = read_sonar(0);
    bottom_sonar_value = read_sonar(1);

    if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
        printf("S1: Medio - S2: Medio\n");
    }
    else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
        printf("S1: Longe - S2: Medio\n");
    }
    else if((top_sonar_value < CLOSE_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
        printf("S1: Perto - S2: Medio\n");
    }
    else if((top_sonar_value < CLOSE_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
        printf("S1: Perto - S2: Perto\n");
    }
    else if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
        printf("S1: Medio - S2: Perto\n");
    }
    else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
        printf("S1: Longe - S2: Perto\n");
    }
    else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value > FAR_DISTANCE)){
        printf("S1: Longe - S2: Longe\n");
    }
    else if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value > FAR_DISTANCE)){
        printf("S1: Medio - S2: Longe\n");
    }
    else{
        printf("S1: Perto - S2: Longe\n");
    }
}

void complete_line(int *lines_completed, int measurement_distance, int total_measurements_per_line){
    int measurements_make_in_line = 0;
    float total_moved = 0;
    float speed = 0;

    while(measurements_make_in_line < total_measurements_per_line){
        while(total_moved < measurement_distance){
            // init timer
            wall_follower();
            // speed = get_speed()
            // stop timer

            // total_moved += calculate_distance(timer, speed);
        }
        // start_drill();
        measurements_make_in_line++;
    }

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

void start_navigation(int total_number_lines, int measurement_distance, int total_measurements_per_line){
    int lines_completed = 0;

    while(lines_completed < total_number_lines){
        complete_line(&lines_completed, measurement_distance, total_measurements_per_line);

        checks_ended_circuit(lines_completed, total_number_lines);

        make_first_curve();
        make_second_curve();

        complete_line(&lines_completed, measurement_distance, total_measurements_per_line);

        checks_ended_circuit(lines_completed, total_number_lines);

        make_big_curve();
    }
}

double total_moved_in_line(){
    return 100.0;
}

int main (int argc, char* argv[]){
    int total_number_lines = 0;
    int total_measurements_per_line = 0;
    int measurement_distance = 0;
    FILE *settings_file = fopen("settings.conf", "r");

    fscanf(settings_file, "%d %d %d", &total_number_lines, &total_measurements_per_line, &measurement_distance);

    char* device = (argc < 2) ? MODEMDEVICE : argv[1];

    uart_init(device);

    start_navigation(total_number_lines, measurement_distance, total_measurements_per_line);

    // fclose(settings_file);

    uart_end();

    return 0;
}