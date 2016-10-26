#include "arduino/tests/comm/rasp_uart.h"
#include "arduino/tests/comm/sensors.c"
#include <stdbool.h>
#include "unistd.h"

#define CLOSE_DISTANCE 10
#define MEDIUM_DISTANCE 15
#define FAR_DISTANCE 20

void start_drill(){
    printf("Ready to start drill!\n");
}

void complete_line(int *lines_completed, int measurement_distance){
    // bool complete_line_drill = false;
    // int total_line_drills =
    // int current_total_drills = 0;
    // float top_sonar_value = 0;
    // float bottom_sonar_value = 0;

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

void wall_follower(){
    float top_sonar_value = 0;
    float bottom_sonar_value = 0;

    int i = 0;
    for(i=0;i < 2000;i++){
        top_sonar_value = read_sonar(0);
        bottom_sonar_value = read_sonar(1);

        if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
            print("%d -- S1: Medio - S2: Medio\n", i);
        }
        else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
            print("%d -- S1: Longe - S2: Medio\n", i);
        }
        else if((top_sonar_value < CLOSE_DISTANCE) && (bottom_sonar_value > CLOSE_DISTANCE && bottom_sonar_value < FAR_DISTANCE)){
            print("%d -- S1: Perto - S2: Medio\n", i);
        }
        else if((top_sonar_value < CLOSE_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
            print("%d -- S1: Perto - S2: Perto\n", i);
        }
        else if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
            print("%d -- S1: Medio - S2: Perto\n", i);
        }
        else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value < CLOSE_DISTANCE)){
            print("%d -- S1: Longe - S2: Perto\n", i);
        }
        else if((top_sonar_value > FAR_DISTANCE) && (bottom_sonar_value > FAR_DISTANCE)){
            print("%d -- S1: Longe - S2: Longe\n", i);
        }
        else if((top_sonar_value > CLOSE_DISTANCE && top_sonar_value < FAR_DISTANCE) && (bottom_sonar_value > FAR_DISTANCE)){
            print("%d -- S1: Medio - S2: Longe\n", i);
        }
        else{
            print("%d -- S1: Perto - S2: Longe\n", i);
        }
    }
}

int main (int argc, char* argv[]){
    int total_number_lines = 0;
    int measurement_distance = 0;
    FILE *settings_file = fopen("settings.conf", "r");

    fscanf(settings_file, "%d %d", &total_number_lines, &measurement_distance);

    char* device = (argc < 2) ? MODEMDEVICE : argv[1];

    uart_init(device);

    // start_navigation(total_number_lines, measurement_distance);

    fclose(settings_file);

    uart_end();

    return 0;
}